#!/usr/bin/env python

"""
Markdown documentation generator and checker.

Generate HTML documentation from Markdown files in the current working
directory, and check that their links are valid.
"""

import BaseHTTPServer
import itertools
import os
import re
import socket
import string
import subprocess
import sys
import urllib2

socket.setdefaulttimeout(30) # seconds

cwd = os.getcwd()
outdir = 'out'
markdown_program = 'markdown'
markdown_extension = '.md'
html_extension = '.html'
href = re.compile('href="([^"]+)"')

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

def find_markdown_sources():
    return sorted([f[:-len(markdown_extension)] for f in os.listdir(cwd)
                   if f.endswith(markdown_extension)])

def start_threadpool():
    from multiprocessing import Pool
    return Pool()

def create_outdir():
    path = os.path.join(cwd, outdir)
    if not os.path.exists(path):
        os.makedirs(path)

def path_to_html(name):
    return os.path.join(cwd, outdir, name + html_extension)

def generate_html(name):
    md_sp = subprocess.Popen(
        [markdown_program, name + markdown_extension],
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
    if not parts[0].endswith(markdown_extension):
        return (link, 'Not a link to an internal markdown file')
    if not os.path.isfile(parts[0]):
        return (link, 'No such markdown file')
    if len(parts) == 1:
        return (link, None)
    if len(parts) > 2:
        return (link, 'Too many hashes in link')
    with open(path_to_html(parts[0][:-len(markdown_extension)]), 'r') as html:
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
