# Use Cases

WebAssembly's [high-level goals](HighLevelGoals.md) define *what* WebAssembly
aims to achieve, and in *which order*. *How* WebAssembly achieves its goals is
documented for [Web](Web.md) and [non-Web](NonWeb.md) platforms. The following
is an unordered and incomplete list of applications/domains/computations that
would benefit from WebAssembly and are being considered as use cases during the
design of WebAssembly.

## Inside the browser

* Better execution for languages and toolkits that are currently cross-compiled
  to the Web (C/C++, GWT, …).
* Image / video editing.
* Games:
  - Casual games that need to start quickly.
  - AAA games that have heavy assets.
  - Game portals (mixed-party/origin content).
* Peer-to-peer applications (games, collaborative editing, decentralized and
  centralized).
* Music applications (streaming, caching).
* Image recognition.
* Live video augmentation (e.g. putting hats on people's heads).
* VR and augmented reality (very low latency).
* CAD applications.
* Scientific visualization and simulation.
* Interactive educational software, and news articles.
* Platform simulation / emulation (ARC, DOSBox, QEMU, MAME, …).
* Language interpreters and virtual machines.
* POSIX user-space environment, allowing porting of existing POSIX applications.
* Developer tooling (editors, compilers, debuggers, …).
* Remote desktop.
* VPN.
* Encryption.
* Local web server.
* Common NPAPI users, within the web's security model and APIs.
* Fat client for enterprise applications (e.g. databases).

# Outside the browser

* Game distribution service (portable and secure).
* Server-side compute of untrusted code.
* Server-side application.
* Hybrid native apps on mobile devices.
* Symmetric computations across multiple nodes

# How WebAssembly can be used

* Entire code base in WebAssembly.
* Main frame in WebAssembly, but the UI is in JavaScript / HTML.
* Re-use existing code by targeting WebAssembly, embedded in a larger
  JavaScript / HTML application. This could be anything from simple helper
  libraries, to compute-oriented task offload.
