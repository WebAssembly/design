#!/usr/bin/env python

"""
Markdown documentation generator and checker.

Generate HTML documentation from Markdown files in the current working
directory, and check that their links are valid.
"""

import BaseHTTPServer
import argparse
import itertools
import os
import re
import socket
import string
import subprocess
import sys
import urllib2

class CheckDir(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        d = values
        if not os.path.isdir(d):
            raise argparse.ArgumentTypeError('Invalid directory "%s"' % d)
        if os.access(d, os.R_OK):
            setattr(namespace, self.dest, d)
        else:
            raise argparse.ArgumentTypeError('Cannot read directory "%s"' % d)

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--socket-timeout', type=int, default=30,
                    help='timeout for URL fetch (seconds)')
parser.add_argument('--dir', type=str, action=CheckDir, default=os.getcwd(),
                    help='Markdown file location (default: current)')
parser.add_argument('--out', type=str, default='out',
                    help='subdirectory for output HTML files')
parser.add_argument('--markdown', type=str, default='markdown',
                    help='Markdown generator program')
parser.add_argument('--extension', type=str, default='.md',
                    help='Markdown file extension')
parser.add_argument('--html', type=str, default='.html',
                    help='HTML file extension')
parser.add_argument('--link_regex', type=str, default='href="([^"]+)"',
                    help='Regular expression used to find links in HTML')
args = parser.parse_args()

socket.setdefaulttimeout(args.socket_timeout)
href = re.compile(args.link_regex)

template = """<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>$title: WebAssembly</title>
</head>
<body>
$content
</body>
</html>
"""

def trunc_extension(filename, extension):
    return filename[:-len(extension)]

def find_markdown_sources():
    return sorted([trunc_extension(f, args.extension) for f in
                   os.listdir(args.dir) if f.endswith(args.extension)])

def start_threadpool():
    from multiprocessing import Pool
    return Pool()

def path_to_markdown(name):
    return os.path.join(args.dir, name + args.extension)

def create_outdir():
    path = os.path.join(args.dir, args.out)
    if not os.path.exists(path):
        os.makedirs(path)

def path_to_html(name):
    return os.path.join(args.dir, args.out, name + args.html)

def generate_html(name):
    md_sp = subprocess.Popen(
        [args.markdown, path_to_markdown(name)],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    md_out, md_err = md_sp.communicate()
    md_code = md_sp.returncode
    if not md_code:
        with open(path_to_html(name), 'w+') as html:
            t = string.Template(template)
            html.write(t.substitute(title=name, content=md_out))
    return (name, md_code, md_err)

def check_generated(generated):
    errors = 0
    for g in generated:
        if g[1]:
            errors = errors + 1
            print '\t%s failed with code %i: %s' % g
        else:
            print '\t%s' % g[0]
    if errors:
        sys.exit(1)

def collect_links_from_html(name):
    html = open(path_to_html(name), 'r').read()
    return re.findall(href, html)

def flatten_and_deduplicate(list_of_lists):
    return sorted(list(set(itertools.chain(*list_of_lists))))

def check_inner_link(link):
    parts = link.split('#')
    if not parts[0].endswith(args.extension):
        return (link, 'Not a link to an internal markdown file')
    name = trunc_extension(parts[0], args.extension)
    md = path_to_markdown(name)
    if not os.path.isfile(md):
        return (link, 'No such markdown file')
    if len(parts) == 1:
        return (link, None)
    if len(parts) > 2:
        return (link, 'Too many hashes in link')
    with open(path_to_html(name), 'r') as html:
        # TODO: The current Markdown generator doesn't output link IDs, whereas
        #       github's Markdown generator does output the IDs.
        return (link, None)

def check_outer_link(link):
    req = urllib2.Request(link)
    try:
        urllib2.urlopen(req)
    except ValueError as e:
        return (link, 'Value error ' + e.args[0])
    except urllib2.URLError as e:
        return (link, 'URL error ' + str(e.reason))
    except urllib2.HTTPError as e:
        responses = BaseHTTPServer.BaseHTTPRequestHandler.responses
        return (link, 'HTTP error ' + responses[e.code()])
    return (link, None)

def print_invalid_links(links):
    errors = 0
    for link in links:
        if link[1]:
            errors = errors + 1
            print '  Invalid link "%s": %s.' % link
    return errors

if __name__ == '__main__':
    sources = find_markdown_sources()
    print 'Found %i markdown sources:' % len(sources)

    pool = start_threadpool()
    create_outdir()

    generated = pool.map(generate_html, sources)
    check_generated(generated)

    links = flatten_and_deduplicate(pool.map(collect_links_from_html, sources))
    inner_links = [l for l in links if l.startswith(tuple(sources))]
    outer_links = [l for l in links if not l.startswith(tuple(sources))]
    print ('Found %i unique inner links, and %i unique outer links.' %
           (len(inner_links), len(outer_links)))
    bad_inner = print_invalid_links(pool.map(check_inner_link, inner_links))
    bad_outer = print_invalid_links(pool.map(check_outer_link, outer_links))
    if not (bad_inner or bad_outer):
        print 'No invalid links.'
    else:
        print 'Found %i bad inner and %i outer links.' % (bad_inner, bad_outer)
        sys.exit(1)
