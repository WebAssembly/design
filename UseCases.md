# Use Cases

WebAssembly's [high-level goals](HighLevelGoals.md) define *what* WebAssembly
aims to achieve, and in *which order*. *How* WebAssembly achieves its goals is
documented for [Web](Web.md) and [non-Web](NonWeb.md) platforms. The following
list of use cases is an unordered and incomplete list of what WebAssembly's
designers think WebAssembly will *enable*.

## Inside the browser

* Common NPAPI users, within the web’s security model.
* Better execution for languages that are currently cross-compiled (GWT, Dart,
  C/C++).
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
* Language interpreters.
* Language virtual machines.
* Entire UNIX user-space environment, with existing UNIX applications.
* Developer tooling (editors, compilers, debuggers, …).
* Remote desktop.
* VPN.
* Encryption.
* Local web server.
* Fat client for enterprise applications (e.g. databases).

# Outside the browser

* Game distribution service (portable and secure).
* Server-side compute of untrusted code.
* Server-side application.
* Hybrid native apps on mobile devices.

# How WebAssembly can be used

* Entire code base in Web Assembly.
* Main frame in Web Assembly, but the UI is in JavaScript / HTML.
* Re-use existing code by targeting Web Assembly, embedded in a larger
  JavaScript / HTML application.
