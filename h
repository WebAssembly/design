[33mcommit 21aeb4ae3463f323ba23496de9d0c6a519b401a7[m[33m ([m[1;36mHEAD[m[33m -> [m[1;32mtable-memory-binary-changes[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Jul 19 09:16:01 2016 -0500

    Add 'end' opcode delimiter to init_expr

[33mcommit 958c19e9018a635fb8764f04f9399742d71791b5[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Jul 19 09:03:20 2016 -0500

    Rename resizable_definition to resizable_limits

[33mcommit 87f86900131983f5d18c22aa372d164e7c48fc3a[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Jul 19 08:48:32 2016 -0500

    Rename definition_kind to external_kind for precision

[33mcommit 2e39974a86d0a55b2f1d7ab10937aae356ee17e7[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Jul 18 17:36:15 2016 -0500

    Fix thinko in import section

[33mcommit bacf98935c5656de28d824b61e58254477b73564[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Jul 18 17:03:41 2016 -0500

    BinaryEncoding.md changes implied by #682

[33mcommit 62f30968a641ef6eb7f8cceadae232cea4ad7b4a[m[33m ([m[1;31morigin/master[m[33m, [m[1;31morigin/HEAD[m[33m, [m[1;32mmaster[m[33m)[m
Author: Dominic Chen <d.c.ddcc@gmail.com>
Date:   Mon Jul 11 21:34:03 2016 -0700

    Provide overview of security model (#717)
    
    This commit introduces a discussion of WebAssembly's security model
    for both users and developers, though primarily targeted at latter.
    Resolves #205.

[33mcommit f69ae0188a1905ee34d0a50f61d848759f5bcbf6[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 8 17:52:07 2016 -0700

    Update references to the "function" element type. (#721)
    
    This is renamed to "anyfunc" in AstSemantics.md in #682.
    
    Also, fix grammar in the Elements Section.

[33mcommit 85c5e1c6de2a7a6fb098529de83eef743cfc7d4a[m
Author: Randy Westlund <rwestlun@gmail.com>
Date:   Thu Jun 30 22:39:15 2016 -0400

    Fix grammatical error (#718)

[33mcommit 56e617e8144a8b750a1198871eb6d43fad182c21[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 28 17:45:04 2016 +0100

    Add JS Memory and Table API, support dynamic linking (#682)
    
    * Add Memory, Table and multi-import/export of these
    
    * Clarify mutation of elements
    
    * Mention CFI use case
    
    * Clarify call_table signature mismatches
    
    * Explicate elements section
    
    * Change support to allow/enable
    
    * Remove table/memory indices add global imports/exports
    
    * Clarify host-defined (i.e., JavaScript) table elements
    
    * s/necessary/useful
    
    * s/will/may/
    
    * Fix typo
    
    * Fix typo
    
    * Clarify that table updates are observed by all instances
    
    * Clarify that host-defined table element values can have different signature checking
    
    * Just 'constraints' since more detail is below
    
    * Wordsmith DynamicLinking.md intro
    
    * Wordsmith FutureFeatures.md
    
    * Try to clarify wording in FutureFeatures.md
    
    * Change 'function' to 'anyfunc'
    
    * Fix nits
    
    * Refine description of initializer expressions
    
    * Remove the Definition Index Space, use (type, index) pairs as necessary

[33mcommit 78644dbdbeee4729664d9e3348397f7ea124c10a[m
Author: rossberg-chromium <rossberg@chromium.org>
Date:   Wed Jun 15 11:04:07 2016 +0200

    Make dropping of values explicit (#694)
    
    Simplify Wasm and allow a more natural interpretation of Wasm as a stack machine. The main changes are:
    
    - Values can no longer be discarded implicitly.
    - Instead, there is an explicit drop operator.
    - To compensate, store operators no longer return a value.
    
    The constructs affected by this are mainly blocks and block-like sequences. Before, all expressions in a block could yield a value, and it would just be dropped on the floor implicitly (except the last one). Now we require explicit drop operations in those cases. With stores no longer returning a value, the only places were we expect drops to actually arise are calls to side-effecting functions that also return a value, but this value isn't used.
    
    (Also fixing a bunch of other out-of-date text on the way.)

[33mcommit 62c8075f9bd5726dc81bd9ef363147a52bf019ab[m
Author: KarlSchimpf <karlschimpf@gmail.com>
Date:   Wed Jun 8 14:33:03 2016 -0700

    Propose possible future layer 1 compression design doc. (#705)
    
    * Propose possible future layer 1 compression design doc.
    
    * Change sentence to explicitly state that compression is a proposal.

[33mcommit fe30995024c050c6c28ebfc13dae6ed4297e8158[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu May 19 11:25:22 2016 +0200

    Fix BinaryEncoding.md to say that the AST encoding is post-order. (#692)

[33mcommit 38a8c079fb0f9ea025c677b68fd6cc622b6dd9ff[m
Author: Liigo Zhuang <liigo@qq.com>
Date:   Tue May 17 15:19:34 2016 +0800

    Fix typos in function bodies (#691)

[33mcommit 76a4ae125ec2fcb7d5c767e4e600873f0d8380f0[m
Author: Kel <humanoidanalog@gmail.com>
Date:   Sat May 14 10:20:07 2016 -0700

    Remove redundant text on quad precision types (#690)

[33mcommit eba746ae6a9f8c3b1bed7cdcd290323f4784bf1f[m
Author: Ben Smith <binjimin@gmail.com>
Date:   Thu May 12 20:14:09 2016 -0700

    Remove link to wasm-e2e from README.md (#689)
    
    It is not maintained, so we shouldn't call it out on the first page you
    see.

[33mcommit 7d1532a40d107fe82fd1ae4724024ca6e66fbe37[m
Author: Lars T Hansen <lars-t-hansen@users.noreply.github.com>
Date:   Wed May 4 16:45:25 2016 +0200

    Clarify type of the immediate offset operand (#680)
    
    * Clarify type of the immediate offset operand
    
    * Refined
    
    * Updated

[33mcommit c7e93c458419d9eb3011926c49a07dd2a23332f9[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 2 21:00:51 2016 -0500

    add initial JS API spec (#659)
    
    * Add basic JS API
    
    * Expand JS in title; link MVP; Unicode mdash; deliminted
    
    * Change 'name' to an index/number
    
    * Reject, not throw, on failure in Wasm.compile
    
    * Loosen wording to allow racy copy of SAB in Wasm.compile
    
    * Mention streams as a future extension of Wasm.compile
    
    * Change TypedArray to BufferSource
    
    * s/receiver/this value/
    
    * Fill in missing builtin-function internal slots
    
    * Simplify FutureFeatures.md sentence, remove typo
    
    * s/Wasm/WASM/
    
    * Switch to WASM.Instance constructor and Module Namespace exports object
    
    * Add note about Wasm.instantiateModule
    
    * s/WASM/WebAssembly/

[33mcommit fa0118f9f6ecd441023bf8b77112aa7a72ce22d9[m
Author: Joshua Olson <0joshua.olson1@gmail.com>
Date:   Sun May 1 16:49:25 2016 -0600

    fix grammar (#679)

[33mcommit 3e2250e9edd3d4914abc65a99e0d05720772583e[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Apr 29 15:15:50 2016 -0700

    Function bodies contain a list of expressions. (#674)

[33mcommit fa6af8657d6d85f3daa7788df011930a080ab5f2[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Apr 29 08:39:18 2016 -0500

    merge binary_0xb (#675)
    
    * Merge pull request #648 from WebAssembly/current_memory
    
    Add current_memory operator
    
    * Reorder section size field (#639)
    
    * Prettify section names (#638)
    
    * Extensible encoding of function signatures (#640)
    
    * Prettify section names
    
    * Restructure encoding of function signatures
    
    * Revert "[Binary 11] Update the version number to 0xB."
    
    * Leave index space for growing the number of base types
    
    * Comments addressed
    
    * clarify how export/import names convert to JS strings (#569) (#573)
    
    * When embedded in the web, clarify how export/import names convert to JS strings (#569)
    
    * Fixes suggested by @jf
    
    * Address more feedback
    
    Added a link to http://monsur.hossa.in/2012/07/20/utf-8-in-javascript.html.  Simplified the decoding algorithm thanks to Luke's feedback.
    
    * Access to proprietary APIs apart from HTML5 (#656)
    
    * comments
    
    * Merge pull request #641 from WebAssembly/postorder_opcodes
    
    Postorder opcodes
    
    * fix some text that seems to be in the wrong order (#670)
    
    * Clarify that br_table has a branch argument (#664)
    
    * Add explicit argument counts (#672)
    
    * Add explicit arities
    
    * Rename
    
    * Replace uint8 with varint7 in form field (#662)
    
    This needs to be variable-length.

[33mcommit 149a43b3124ed9d7a8385757976f766324ab4c80[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 28 11:33:26 2016 -0700

    Permit implementations to return canonical NaNs from arithmetic. (#660)
    
    * Permit implementations to return canonical NaNs from arithmetic.
    
    RISC-V and ARM in "default NaN" mode do not propagate NaN values in
    arithmetic operations. Instead, they return a "canonical" NaN.
    
    IEEE 754 doesn't require NaN propagation; it's merely a "should".
    
    In order to ensure that wasm doesn't accidentally over-burden support
    for the above and other potential future IEEE-754-conforming platforms,
    this patch changes the NaN rules to permit either behavior.
    
    * Add the IEEE 754 section number for the NaN propagation recommendation.
    
    * Avoid "typically".
    
    * Reword the NaN conversion rules for clarity.

[33mcommit f850552dd07b940ef45d7eee7d8421bb07dd98ec[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Apr 27 18:16:38 2016 -0700

    Merge pull request #608 from WebAssembly/rationale-revisions
    
    Revise, update, and expand several Rationale entries.

[33mcommit 477274504ea60c2f06ee195bff36c71d24aefaf8[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 21 16:55:59 2016 -0500

    Rename fields in memory section (#661)
    
    * Rename fields in memory section
    
    Does not affect the spec, but I think `initial` is a better name than `minimum` since the `initial` isn't necessarily the `minimum` if, in the future, we were to add a `shrink_memory` + `minimum` (which could be <= `initial`).  The PR also renames `max` to be more consistent with other names in this file.
    
    * Update BinaryEncoding.md

[33mcommit abb6c9b715579045696ff8e72f4d2ab542c34d9d[m
Author: Benjamin Bouvier <public@benj.me>
Date:   Thu Apr 21 23:54:50 2016 +0200

    Merge pull request #665 from bnjbvr/patch-2
    
    Fix typo in Web.md

[33mcommit 2312afd61751bd995341fd477381a40defb79742[m
Author: Alexander Orlov <alexander.orlov@loxal.net>
Date:   Fri Apr 15 22:18:57 2016 +0200

    Access to proprietary APIs apart from HTML5 (#656)

[33mcommit 04c63fb2b26f25b2bfd88569baf8711f146e4d06[m
Author: pizlonator <pizlo@mac.com>
Date:   Fri Apr 15 13:17:37 2016 -0700

    clarify how export/import names convert to JS strings (#569) (#573)
    
    * When embedded in the web, clarify how export/import names convert to JS strings (#569)
    
    * Fixes suggested by @jf
    
    * Address more feedback
    
    Added a link to http://monsur.hossa.in/2012/07/20/utf-8-in-javascript.html.  Simplified the decoding algorithm thanks to Luke's feedback.

[33mcommit 0ac338d6ebbfc47613d9b63e105d2d896c714ce7[m
Merge: 247d7b6 7c8be53
Author: titzer <titzer@google.com>
Date:   Tue Apr 5 16:52:48 2016 +0200

    Merge pull request #642 from WebAssembly/revert-637-binary_0xb_version
    
    Revert "[Binary 11] Update the version number to 0xB."

[33mcommit 7c8be53bad55836ac5f0c6d1de70006552162901[m
Author: titzer <titzer@google.com>
Date:   Tue Apr 5 16:51:22 2016 +0200

    Revert "[Binary 11] Update the version number to 0xB."

[33mcommit 247d7b671d0ed79d41b9b4d2ede659360e69e37f[m
Merge: e55d280 f4b772b
Author: titzer <titzer@google.com>
Date:   Tue Apr 5 16:51:06 2016 +0200

    Merge pull request #637 from WebAssembly/binary_0xb_version
    
    [Binary 11] Update the version number to 0xB.

[33mcommit f4b772b16ea0c60d545c6c3fb14a8dfcbc403a1d[m
Author: titzer <titzer@google.com>
Date:   Tue Apr 5 15:05:46 2016 +0200

    Update BinaryEncoding.md

[33mcommit 3e1b6db38e3cc1c07e479ea58905d260062a7ee4[m
Author: titzer <titzer@google.com>
Date:   Tue Apr 5 15:03:14 2016 +0200

    Reword the magic number and version.

[33mcommit 1f330a01ce7a351a9f2ebb45ae49e8633fab2674[m
Author: titzer <titzer@google.com>
Date:   Tue Apr 5 15:01:43 2016 +0200

    This one goes to 11.

[33mcommit e55d2806490ca37abdef6afd0bc09169215482fe[m
Merge: 58d5ad2 832e67b
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Apr 1 18:06:04 2016 -0700

    Merge pull request #633 from WebAssembly/jfbastien-patch-1
    
    Mention extreme exhaustion

[33mcommit 58d5ad28712fc634a0f3dbe443177410879ae6da[m
Merge: 8d067ee 3c5d205
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Apr 1 16:54:54 2016 -0500

    Merge pull request #629 from WebAssembly/update-memory-max
    
    make the maximum memory size a hard limit and update grow_memory return value

[33mcommit 832e67b370c38353fdf19136c205cd97d015ee8a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Mar 31 16:20:22 2016 -0700

    Mention extreme exhaustion

[33mcommit 8d067ee68e4544d18b0c29d6bdb34dde6e1c125e[m
Merge: 5b91ef6 675eee7
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Mar 31 16:18:40 2016 -0700

    Merge pull request #632 from WebAssembly/jfbastien-patch-1
    
    Clarify physical page exhaustion

[33mcommit 675eee78ea4613025ec3bd12f0020609bfb8c81a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Mar 31 15:37:53 2016 -0700

    Clarify physical page exhaustion
    
    As discussed in https://github.com/WebAssembly/design/pull/629#discussion_r58138148

[33mcommit 5b91ef68a3fbcf0655e6c3e21d0f87e6cdfc0c58[m
Merge: 1c9de01 77e91fc
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Mar 31 15:12:45 2016 -0700

    Merge pull request #631 from wanderer/patch-3
    
    Added "symmetric computations across multiple nodes"

[33mcommit 77e91fc1bfd42189cf9dec106e6590256ad0c3d3[m
Author: wanderer <wanderer@users.noreply.github.com>
Date:   Thu Mar 31 17:47:53 2016 -0400

    Added "symmetric computations across multiple nodes"

[33mcommit 3c5d2052a6febae095192d1ff652a54a553b3d00[m[33m ([m[1;31morigin/update-memory-max[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Thu Mar 31 13:45:37 2016 -0500

    Clarify failure conditions in AstSemantics.md

[33mcommit 0918d83d23aa421e956dfbcaeefd9faeae94237e[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 30 18:11:50 2016 -0500

    Change max to optional and hard

[33mcommit 1c9de01a77a66279e9c7a20cd580a74a5ffbc511[m
Merge: b81e46a 1083d2c
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Mar 31 09:16:22 2016 -0500

    Merge pull request #597 from WebAssembly/precise-ordering
    
    Define precise order of sections

[33mcommit b81e46aac37d8633d1c3470f5db4933aa2834e15[m
Merge: 7af4b47 01a42d5
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Mar 28 10:00:57 2016 -0700

    Merge pull request #627 from naturaltransformation/patch-12
    
    Reserved bitfield minor word change

[33mcommit 7af4b479e34034d7e46846555ae0335f96084e14[m
Merge: 5f338c6 87f8894
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Mar 28 11:50:21 2016 -0500

    Merge pull request #598 from JSStats/grow-in-pages
    
    Change the argument to grow memory to be in units of pages.

[33mcommit 01a42d5121d07af324bd1aed18e8115797457a22[m
Author: George Kuan <george.kuan@intel.com>
Date:   Mon Mar 28 09:44:06 2016 -0700

    Reserved bitfield minor word change

[33mcommit 5f338c6501971bde7575125c03078399db08532a[m
Merge: db74841 67618e9
Author: rossberg-chromium <rossberg@chromium.org>
Date:   Thu Mar 24 12:35:32 2016 +0100

    Merge pull request #606 from naturaltransformation/patch-10
    
    Forward compatibility for unused bitfield bits

[33mcommit db74841c7094c62ede9f1ea427e38ff152d599e5[m
Merge: b6398a3 74cf0f1
Author: Ben Smith <binjimin@gmail.com>
Date:   Sat Mar 19 15:08:06 2016 -0700

    Merge pull request #614 from lars-t-hansen/sab_ref_fix
    
    Correct reference to SharedArrayBuffer proposal

[33mcommit b6398a38f07371f2b7fa4c3c3d2888e3017d704c[m
Merge: b52cc14 d885b29
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Mar 19 09:14:26 2016 -0700

    Merge pull request #615 from WebAssembly/fix-feature-test
    
    Fix feature testing

[33mcommit b52cc140f2088a17af04d2b966590130b3a20266[m
Merge: 6ff5972 832aaa8
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Mar 18 15:00:51 2016 -0700

    Merge pull request #610 from WebAssembly/wasm-object
    
    Skeleton for the Wasm object

[33mcommit 832aaa80a03cd6e30b53320f3e77d5be42f2e9ff[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Mar 18 14:59:37 2016 -0700

    Mention ES6 modules

[33mcommit d885b2939a6d63d41af7379246aaa106b5f30167[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Mar 18 10:14:46 2016 -0700

    Fix feature testing
    
    It was removed in #523, but was still referenced. Leave it as a high-level sketch for now.
    
    Fixes #613.

[33mcommit 74cf0f110f6c09d4bec87d9966708d719be5c351[m
Author: Lars T Hansen <lth@acm.org>
Date:   Fri Mar 18 13:50:32 2016 +0100

    Correct reference to SharedArrayBuffer proposal

[33mcommit 6ff5972b371af4d2011b6d97fc883ca9ddf10892[m
Merge: 5d27aa7 74c98ee
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Mar 17 11:01:15 2016 -0700

    Merge pull request #612 from faineance/patch-1
    
    AstSemantics dead link in BinaryEncoding

[33mcommit 74c98ee636844aea10610e9b77767eef2ca07982[m
Author: Anthony <faineance@users.noreply.github.com>
Date:   Thu Mar 17 17:51:01 2016 +0000

    AstSemantics dead link in BinaryEncoding

[33mcommit 5d27aa7f71fb1086a79fcb5c436724a0d75bf378[m
Merge: 76cb68f 54f56be
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Mar 15 11:19:47 2016 -0700

    Merge pull request #609 from WebAssembly/limited-dynamic-linking
    
    Mention limited dynamic linking

[33mcommit 54f56be94a70640247d70461bea07c330f639da3[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Mar 15 11:19:40 2016 -0700

    Address comments

[33mcommit d48ad63b0ca2a2e75b006260f4fd15bf5171e00a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Mar 15 11:13:45 2016 -0700

    Update DynamicLinking.md

[33mcommit fd1aab9fc11d2d92674ff08f94470d741d7f3e4a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Mar 15 10:54:52 2016 -0700

    Skeleton for the Wasm object
    
    We should document this better. Here's a start.

[33mcommit 21c6c37f01c69e5c0e49ec7cb66489fffca1a1fb[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Mar 15 10:49:27 2016 -0700

    Mention limited dynamic linking
    
    Further clarifications will be needed to explain the Wasm object's API. That should be done in a separate PR.

[33mcommit 1083d2c74858bca789fdc44706dff726f4d98e79[m[33m ([m[1;31morigin/precise-ordering[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 14 09:23:06 2016 -0500

    Remove End section again

[33mcommit d0b57871178c8ce6cf25845d62f4a6c11d0fc9a3[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 14 09:22:13 2016 -0500

    Revert "Update size of smallest module"
    
    This reverts commit 4c310f2c4e7e2560fa0225b0bb2cc7f4c8ab3dde.

[33mcommit 7f8b597cdfb2b6a0f8ac65177c27e4bf9ddfd902[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 14 09:21:56 2016 -0500

    Revert "No validation errors after the End"
    
    This reverts commit e3375281d46fe39cc1f37a4cd02cc524d3c8fdc6.

[33mcommit e3375281d46fe39cc1f37a4cd02cc524d3c8fdc6[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Sun Mar 13 19:35:42 2016 -0500

    No validation errors after the End

[33mcommit 4c310f2c4e7e2560fa0225b0bb2cc7f4c8ab3dde[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Sat Mar 12 15:12:54 2016 -0600

    Update size of smallest module

[33mcommit e8fb2df76cb065c1fd5d3804a50fb202ee090c69[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 9 15:07:43 2016 -0600

    Define precise order of sections

[33mcommit 67618e9aa09589231684175fdb77034085f8a8ec[m
Author: George Kuan <george.kuan@intel.com>
Date:   Fri Mar 11 15:36:53 2016 -0800

    Forward compatibility for unused bitfield bits
    
    The remaining bits in the flag are underspecified. We should consider specifying them.

[33mcommit 76cb68fc292ee793434af34bd4f21905f017f3d8[m
Merge: 9925904 3d0c861
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 11 13:09:31 2016 -0800

    Merge pull request #605 from commonlisp/patch-1
    
    Typo that messes up the opcode table

[33mcommit 3d0c8616ce23573895de6d47e056e92a25ddfe04[m
Author: George Kuan <commonlisp@users.noreply.github.com>
Date:   Fri Mar 11 13:08:06 2016 -0800

    Typo that messes up the opcode table

[33mcommit 9925904e2854f421b476628547c9d7b61da70fac[m
Merge: f77a899 f66cdbb
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Mar 11 10:34:24 2016 -0800

    Merge pull request #600 from wanderer/patch-1
    
    Update Events.md

[33mcommit f77a899dad3fe75ccae0628fef875272ca73c24b[m
Merge: 6ded592 1c5618c
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 11 09:58:37 2016 -0800

    Merge pull request #603 from naturaltransformation/patch-8
    
    Minor transposition typos

[33mcommit 1c5618ccb0aecf4a41ba72f9aac9a5cbe96fd3fd[m
Author: George Kuan <george.kuan@intel.com>
Date:   Fri Mar 11 09:57:57 2016 -0800

    Minor transposition typos

[33mcommit 6ded59220b12cdffdfa1050fe162f0627bb4b73b[m
Merge: e6021f0 f0bcd90
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Mar 10 07:04:47 2016 -0800

    Merge pull request #599 from WebAssembly/eqz-encoding
    
    Encodings for i32.eqz and i64.eqz.

[33mcommit f66cdbb321a91aea239bc83205a0b5ab5fe531ae[m
Author: wanderer <wanderer@users.noreply.github.com>
Date:   Wed Mar 9 22:41:57 2016 -0500

    Update Events.md

[33mcommit f0bcd90aa0151b1033340c9161a53b79511caa5d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Mar 9 19:15:21 2016 -0800

    Encodings for i32.eqz and i64.eqz.

[33mcommit 87f88940cc49efeea6df618ed237f42730e30dc3[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Thu Mar 10 13:59:05 2016 +1100

    Change the argument to grow memory to be in units of pages.

[33mcommit e6021f003b1dd5b1d37f4dfe4d7d97e8c60dbba7[m
Merge: 74caec7 cabbe0c
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Mar 9 18:15:22 2016 -0800

    Merge pull request #583 from JSStats/rotl-rotr-encoding
    
    Add the rotate operator encodings.

[33mcommit cabbe0c4dbf7ec946893b81b2f9605a0ec48740f[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Thu Mar 3 16:39:53 2016 +1100

    Add the rotate operator encodings.
    
    Corresponds to https://codereview.chromium.org/1755013003/

[33mcommit 74caec7110a97ebe2dde3ffa7d1971cdcdc1611d[m
Merge: 1df7880 67c28db
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Mar 9 13:09:25 2016 -0600

    Merge pull request #592 from WebAssembly/leb128-all-the-things
    
    Convert (most) remaining fixed-width immediates to LEB128

[33mcommit 1df788016ee70b04d14a8d1852343c63a4f877d3[m
Merge: d678a13 b3f5fb9
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Mar 9 13:09:06 2016 -0600

    Merge pull request #591 from WebAssembly/split-functions
    
    Split Functions section into Function Signatures and Function Bodies

[33mcommit 67c28db3a46d639b8385e35daee9e294c0d84acf[m[33m ([m[1;31morigin/leb128-all-the-things[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 9 13:07:32 2016 -0600

    Add back the varuint32 in data_segment

[33mcommit d678a1350fc1ce980dc7699264976955d825c8ce[m
Merge: b806cc3 2470472
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Mar 9 10:14:04 2016 -0800

    Merge pull request #555 from WebAssembly/not
    
    Add i32.eqz.

[33mcommit a3ace6fb6e54ea2a7612d77745c590e4a90c09b7[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 9 10:50:52 2016 -0600

    Revert "Opcodes should also be LEB128; we're definitely going to end up with >256 after SIMD"
    
    This reverts commit edac92d9e10e5fb3ea8e305236c6d00da05a8d48

[33mcommit 369abe553e96dc2ebbe847f4c6a76f05e5c6ef2d[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 9 00:08:51 2016 -0600

    Opcodes should also be LEB128; we're definitely going to end up with >256 after SIMD

[33mcommit 9632db5411c6812b4eb83c1397b1463fae442556[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Mar 8 19:13:09 2016 -0600

    Clarify alignment description

[33mcommit c6e52a487a81edd9ccdd7544bd96c367aff991ba[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Mar 8 13:05:13 2016 -0600

    Change br_table branch offsets to uint32

[33mcommit 3d1fc545dc6b61150021b93fff91895cb4679e94[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Mar 8 12:18:47 2016 -0600

    Update flags/alignment immediate

[33mcommit b1fa2cb742d31211cc4d24b758497683f1e42089[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 12:46:08 2016 -0600

    Convert remaining fixed-width integers to varuint32

[33mcommit b3f5fb96debb55cd6e978421cde63585a38a303d[m[33m ([m[1;31morigin/split-functions[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 20:03:23 2016 -0600

    Update references to Function section to Function Signatures section

[33mcommit f0ab07c4d53932fc4e812b2d43d1eeac37b1acd4[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 16:47:32 2016 -0600

    Add back function body size

[33mcommit 29c4de983a67be2dd9a2d8985cb39d122501d71b[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 12:58:53 2016 -0600

    Address comments

[33mcommit 2ead607c0e2298a81dc27d2b04b114242fc48634[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 12:47:53 2016 -0600

    Remove now-unnecessary signature index from export

[33mcommit 5fa18dd0e445d542a0e3c31fd00946e6d40e9825[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 12:11:39 2016 -0600

    Split Functions section into Function Signatures and Function Bodies

[33mcommit b806cc344a48f8e4ec610bae2695b94be8b7f8bb[m
Merge: 2001bcf ceb79f1
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Mar 9 06:58:31 2016 -0800

    Merge pull request #589 from WebAssembly/load_global
    
    Remove the nonstandard load_global and store_global opcodes.

[33mcommit 2001bcfbb33578e3e0d546a4edf38bc2c149aaf0[m
Merge: 2e5548b bf2aafc
Author: titzer <titzer@google.com>
Date:   Wed Mar 9 08:17:24 2016 +0100

    Merge pull request #590 from WebAssembly/inline-strings
    
    Store section data inline in the section

[33mcommit 2e5548b0ea58becff293e8028c2b06a68f1f738a[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 9 00:07:17 2016 -0600

    Revert "Opcodes should also be LEB128; we're definitely going to end up with >256 after SIMD"
    
    This reverts commit b3378e3e6621256ca81e4c9a8f9ac858d6e69a5d (accidental push to master)

[33mcommit d2b487a60dafacfe2a4683c655326e3cdd45d8c0[m
Merge: b3378e3 c40dbe8
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 9 00:06:25 2016 -0600

    Merge branch 'master' of https://github.com/WebAssembly/design

[33mcommit b3378e3e6621256ca81e4c9a8f9ac858d6e69a5d[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 9 00:06:07 2016 -0600

    Opcodes should also be LEB128; we're definitely going to end up with >256 after SIMD

[33mcommit bf2aafc8fd81bc7e825a99998691429e84dbed2c[m[33m ([m[1;31morigin/inline-strings[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Mar 8 12:34:10 2016 -0600

    Add the actual inline bytes

[33mcommit c40dbe83c881a5f608489a0cbad28be0056b1b48[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Mar 7 19:27:59 2016 -0800

    Fix one more dangling reference to `tableswitch`.

[33mcommit c0c0dc12f60f4fd6413065015becab351e095d3c[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 11:45:32 2016 -0600

    Store data segments inline

[33mcommit bbc5df6284c8f6230e320c2afd387ea9054e28cc[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 11:41:24 2016 -0600

    Store strings inline in the import/export entries

[33mcommit f09513b37079553854bfce83155d2add3bef4a14[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Mar 7 11:23:06 2016 -0600

    Fix dangling uses of tableswitch to match renaming to br_table in #586

[33mcommit d7bde57191ed15a97e9b357b2c16fa908f25e680[m
Merge: 30fdfe8 c08726c
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Mar 7 10:01:24 2016 -0600

    Merge pull request #580 from WebAssembly/name-section
    
    Names section

[33mcommit 30fdfe890aa6ad904238c16f0ee77d9d36877f6b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 4 17:54:08 2016 -0800

    Change call_import to 0x1f.

[33mcommit ceb79f177722f3bf75fdf91e120f1a6b0d4e3bc7[m[33m ([m[1;31morigin/load_global[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 4 12:00:46 2016 -0800

    Remove the nonstandard load_global and store_global opcodes.

[33mcommit 0146d348c19c6f4032f6222eeabddfc6648cb93d[m
Merge: 50d18c8 78472b4
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 4 11:04:04 2016 -0800

    Merge pull request #588 from WebAssembly/call-import
    
    Add a call_import opcode

[33mcommit 78472b4c501815dec03d7b2cda8a25093b489359[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 4 10:45:52 2016 -0800

    Rename call_function to call, to match AstSemantics.md.

[33mcommit 4cf0c392c171fde1ea2bf1aeb415bfd828bf0a63[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 4 10:45:21 2016 -0800

    Add an opcode for call_indirect.

[33mcommit 50d18c8c0268c730d9af8f8cc65de732fe8e05a9[m
Merge: cbf2a8d d0c0f94
Author: titzer <titzer@google.com>
Date:   Fri Mar 4 10:36:12 2016 -0800

    Merge pull request #586 from WebAssembly/br_table
    
    Replace tableswitch with br_table.

[33mcommit cbf2a8dfd277b70a62f9f56f100b002763d1cbff[m
Merge: 4a4d492 ad46b57
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 4 10:27:13 2016 -0800

    Merge pull request #587 from WebAssembly/binary-op-names
    
    Update opcode names to match AstSemantics.md.

[33mcommit ad46b57c2a69b36ba3f7a54eaa640cbba07379cb[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Mar 4 10:16:41 2016 -0800

    Update opcode names to match AstSemantics.md.

[33mcommit d0c0f94022674bafb97f76ba34fc4f01d775f740[m[33m ([m[1;31morigin/br_table[m[33m)[m
Author: titzer <titzer@google.com>
Date:   Fri Mar 4 09:53:13 2016 -0800

    Add default target

[33mcommit c08726cc0f25a0988277b3e1d7a0765fdf24f527[m[33m ([m[1;31morigin/name-section[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Mar 4 10:37:27 2016 -0600

    Fix typo

[33mcommit 9cb14e31abad1a2ac950e42667a408811b65c5f7[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Mar 4 10:36:36 2016 -0600

    Rename 'names' to 'entries'

[33mcommit 3169f00ad637510a3cd0c9ab928785cbddb0271d[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Mar 4 10:23:58 2016 -0600

    Fuse into one section, give arrays lengths, allow lengths to differ

[33mcommit 0b987cf8d04b563adbdb0822774c8eb972cdb83d[m
Author: titzer <titzer@google.com>
Date:   Fri Mar 4 08:19:41 2016 -0800

    Replace tableswitch with br_table.

[33mcommit 24704728cdef48c61652d3afe3ff685a3adc310f[m[33m ([m[1;31morigin/not[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Feb 25 14:15:15 2016 -0800

    Add i32.eqz and i64.eqz.

[33mcommit 8939858859294e927d7d29309044feee23909a03[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Mar 4 00:37:52 2016 -0600

    Remove non-null requirement

[33mcommit 8ba66de02f123ae647f018f4589ede031f72bbcb[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 2 10:04:12 2016 -0600

    Switch to up-front length from null-terminator

[33mcommit db3dadda0c700372355106312943a77a71c7cacd[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Mar 2 09:55:41 2016 -0600

    s/observable/execution/

[33mcommit 97e54b55d6a1f76d9466fe3d6be8a49e61f5ab01[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Mar 1 18:57:38 2016 -0600

    Mention 0 or 1 times

[33mcommit 0d1393dd3de17f2d5e982853582e322eb4f41a14[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Mar 1 17:16:03 2016 -0600

    Names section

[33mcommit 4a4d492356864a77c63045d84eee2c23e3251992[m
Merge: 8fe4669 6ad3767
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Mar 2 18:38:01 2016 -0600

    Merge pull request #581 from WebAssembly/rm-globals
    
    Remove nonstandard Globals section

[33mcommit 8fe466975b514feffacffe2df52e3cfc2817895e[m
Merge: e8f8df2 cfae7ce
Author: titzer <titzer@google.com>
Date:   Wed Mar 2 13:29:01 2016 -0800

    Merge pull request #572 from WebAssembly/locals_to_func_body
    
    Move local entries to be part of the function body.

[33mcommit e8f8df239a748a5bb7f9dc50cd788fc52d1e98fe[m
Merge: e7c0ac5 1209c31
Author: Ben Smith <binjimin@gmail.com>
Date:   Wed Mar 2 10:00:46 2016 -0800

    Merge pull request #579 from WebAssembly/const-leb
    
    Remove i8.const and make {i32,i64}.const varints.

[33mcommit 6ad3767469f088a7470d1588e7970d06ce34499d[m[33m ([m[1;31morigin/rm-globals[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Mar 1 19:49:54 2016 -0600

    Remove nonstandard Globals section

[33mcommit 1209c31424fdd5713e1bf1b889c83e0a86a83c76[m
Author: Ben Smith <binji@chromium.org>
Date:   Tue Mar 1 15:05:53 2016 -0800

    Remove i8.const and make {i32,i64}.const varints.

[33mcommit e7c0ac5aac60fb7745532074a8a38ac0bbec6bc4[m
Merge: 28911a2 e7fcfb8
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Mar 1 16:58:48 2016 -0500

    Merge pull request #574 from naturaltransformation/patch-4
    
    Moving magic number value from Type to Desc column

[33mcommit 28911a25114a4543a1eef36259e6d0341e904168[m
Merge: 88d3303 9b2f1ec
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Mar 1 16:58:24 2016 -0500

    Merge pull request #575 from naturaltransformation/patch-5
    
    Minor typo

[33mcommit 9b2f1ecf552866ff2e42d774ee477a5c7946e2cf[m
Author: George Kuan <george.kuan@intel.com>
Date:   Tue Mar 1 13:49:05 2016 -0800

    Minor typo

[33mcommit e7fcfb8a5d07933a649cadea641e88d403f1f384[m
Author: George Kuan <george.kuan@intel.com>
Date:   Tue Mar 1 13:48:14 2016 -0800

    Moving magic number value from Type to Desc column
    
    It is confusing to put the magic number value as part of the type column.

[33mcommit cfae7ce7c0b8788776d19f73225c595681d4e0c4[m[33m ([m[1;31morigin/locals_to_func_body[m[33m)[m
Author: titzer <titzer@google.com>
Date:   Tue Mar 1 08:25:04 2016 -0800

    Move local entries to be part of the function body.

[33mcommit 88d3303877586d2fd8d7a41415ea4e05e1961820[m
Merge: c8137d9 19b7cf9
Author: rossberg-chromium <rossberg@chromium.org>
Date:   Tue Mar 1 12:12:41 2016 +0100

    Merge pull request #546 from WebAssembly/locals
    
    Binary: Condense format of local declarations

[33mcommit 19b7cf90f07ad23a0261f775f301febab753edfa[m
Merge: 13e0758 c8137d9
Author: Andreas Rossberg <rossberg@chromium.org>
Date:   Tue Mar 1 12:11:42 2016 +0100

    Merge branch 'master' into locals

[33mcommit c8137d9a6bdefd015f5daadd3f0eb233dc6996b6[m
Merge: 61e60c5 18da0d7
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Feb 29 09:03:13 2016 -0500

    Merge pull request #462 from mbodart/rotate
    
    Move rotate instructions from FutureFeatures to AstSemantics.

[33mcommit 61e60c57b34a360d43cbe6c46d9d96f018ae00d9[m
Merge: 35f2995 1495215
Author: JF Bastien <github@jfbastien.com>
Date:   Sun Feb 28 22:13:48 2016 -0500

    Merge pull request #563 from JSStats/magic-encoding
    
    Document the magic number and version.

[33mcommit 14952154322a6508cfc2a8eb1942df9a117d8e0e[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Sun Feb 28 16:13:08 2016 +1100

    Note that the version number will be reset to 1 for MVP.

[33mcommit 5c49b34d3562ec32228177fb71f17a85e583a60b[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Sun Feb 28 12:30:37 2016 +1100

    Document the magic number and version.
    
    As implemented in https://codereview.chromium.org/1740373002/

[33mcommit 35f2995981e5103db6b9fe77ef045eb69fdae3da[m
Merge: d198d6b 531a3cd
Author: titzer <titzer@google.com>
Date:   Sat Feb 27 16:36:39 2016 -0800

    Merge pull request #560 from JSStats/export-section-encoding
    
    Document the export table section.

[33mcommit 531a3cde90728ae59e247c1a0dca7025d9caa01b[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Sat Feb 27 14:03:19 2016 +1100

    Document the export table section.
    
    As implemented in https://codereview.chromium.org/1744713003/

[33mcommit d198d6bfff4c267b573a6f00efda9f51a96d9d38[m
Merge: 9269776 d6898ab
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Feb 27 11:48:14 2016 -0800

    Merge pull request #562 from mbebenita/mbebenita-varuint32
    
    Clarify encoding of varuint32 values.

[33mcommit d6898abafa98ff889454cfb11725d9ec3c9536b5[m
Author: Michael Bebenita <mbebenita@gmail.com>
Date:   Sat Feb 27 10:01:37 2016 -0800

    Clarify encoding of varuint32 values.

[33mcommit 926977694b3cd294c6c8e5f3aac763b71ace55af[m
Merge: dc04371 e6102f6
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Feb 27 08:44:50 2016 -0800

    Merge pull request #559 from JSStats/start-function-encoding
    
    Start function section encoding

[33mcommit e6102f6e5dfbaffae8692e974e4e588fd9adfdef[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Sat Feb 27 13:26:31 2016 +1100

    Document the Start Function section.

[33mcommit dc043717233fb21519c224ef098864afc831e022[m
Merge: 45bcdac 7c06cda
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Feb 26 17:33:54 2016 -0800

    Merge pull request #557 from JSStats/named-sections
    
    Name the sections by strings, and add a size for all sections.

[33mcommit 7c06cda04a9e55e2434be656fce5468e6011c807[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Fri Feb 26 12:42:50 2016 +1100

    Name the sections by strings, and add a size for all sections.

[33mcommit 45bcdac3045029aed30c456f5107dd56e0c55f0e[m
Merge: 4dec284 54574ed
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Feb 26 15:13:50 2016 -0800

    Merge pull request #512 from WebAssembly/no-cases
    
    Remove cases from `tableswitch`.

[33mcommit 4dec2849ad6d79fae63e7a8529da9358ac8cffe8[m
Merge: 4bb16aa 8802362
Author: titzer <titzer@google.com>
Date:   Thu Feb 25 10:11:03 2016 -0800

    Merge pull request #553 from WebAssembly/nop_rationale
    
    Add rationale for nop.

[33mcommit 4bb16aa96fa8d207fede891f6764781095b56bcf[m
Merge: 8ab0786 f42df8d
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Feb 25 09:57:58 2016 -0800

    Merge pull request #547 from WebAssembly/import_table
    
    Add import table.

[33mcommit 880236230f2b20a3096c2fa76aaec3510c545d64[m
Author: titzer <titzer@google.com>
Date:   Thu Feb 25 09:47:10 2016 -0800

    Add rationale for nop.

[33mcommit f42df8dc1ed183c9178577b451e18ebc37cc314d[m
Author: titzer <titzer@google.com>
Date:   Thu Feb 25 09:43:53 2016 -0800

    Update text: "offset from start of the module"

[33mcommit 8ab07860e873c37f5ee724c7a257349cd6aa318a[m
Merge: cbb5902 a7d67f1
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Feb 25 09:09:47 2016 -0800

    Merge pull request #552 from WebAssembly/jfbastien-patch-1
    
    Unknown sections

[33mcommit a7d67f1f03d5000c1a036499d932327060349e24[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Feb 25 09:04:28 2016 -0800

    Unknown sections
    
    Fixes #208.
    
    A later PR will make sure all sections start with the same format (so all sections use bytes, instead of count, making it easy to know the byte count of unknown sections). We'll also use strings instead.

[33mcommit cbb59025710350a5884acd3ce9a284f7fdf88065[m
Merge: 9fc44fc 5fc165f
Author: titzer <titzer@google.com>
Date:   Wed Feb 24 17:10:58 2016 -0800

    Merge pull request #523 from WebAssembly/remove_has_feature
    
    Remove has_feature

[33mcommit 9fc44fcc23dc620accef4977da5677d9041a257e[m
Merge: 25d26eb 0f3104d
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Feb 24 15:04:40 2016 -0800

    Merge pull request #549 from WebAssembly/select
    
    Fix terminology nit

[33mcommit 0f3104dcbcb0ee7c1262aff7e06900e01ce0dfbe[m
Author: Andreas Rossberg <rossberg@chromium.org>
Date:   Thu Feb 25 00:01:31 2016 +0100

    Fix terminology nit

[33mcommit 25d26eb059b18693a3ed0e567d567330a23988d3[m
Merge: 01e8ac7 0170464
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Feb 23 08:33:41 2016 -0800

    Merge pull request #545 from JSStats/mem-pages-encoding
    
    Encode the memory sizes in units of pages, and use a leb128 encoding.

[33mcommit 13e0758ad4ca3f880c3b7fb3afe41ad3377308dd[m
Author: Andreas Rossberg <rossberg@chromium.org>
Date:   Tue Feb 23 17:26:59 2016 +0100

    Eps

[33mcommit 27cfc727482d52ce8c133a95ac655418ad164812[m
Author: titzer <titzer@google.com>
Date:   Tue Feb 23 08:16:41 2016 -0800

    Add import table.

[33mcommit edd010eac01d735e17c91752afc29ed91765bd23[m
Author: Andreas Rossberg <rossberg@chromium.org>
Date:   Tue Feb 23 16:03:27 2016 +0100

    Binary: Condense format of local declarations

[33mcommit 01e8ac76a5d12c8308c3db92242c042ee59f668b[m
Merge: aea4807 c96a7cf
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Feb 22 09:00:37 2016 -0800

    Merge pull request #544 from WebAssembly/grow-memory-return
    
    Make `grow_memory` return the old memory size.

[33mcommit 01704645e0f96dee6cdb86c15b1d2896c1125a6e[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Mon Feb 22 10:03:26 2016 +1100

    Encode the memory sizes in units of pages, and use a leb128 encoding.

[33mcommit c96a7cf112a105b5b539c4cc674c752823c971b0[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Sat Feb 20 13:02:29 2016 -0800

    Add rationale text for `grow_memory` returning the old size.

[33mcommit 953a1b0e7bb9f5cdcb1d3e2718bf2a5779c6fec2[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Feb 19 16:19:21 2016 -0800

    Make `grow_memory` return the old memory size.
    
    This implements the discussion in bug #525.

[33mcommit aea48078aef9fbcaf52b3f728c2231b2fe375437[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Feb 18 11:55:40 2016 -0800

    Fix spelling errors.

[33mcommit e1b791c2bd0862a42c0533cfe04ddd873ab937ac[m
Merge: 413c8bc 0df869b
Author: titzer <titzer@google.com>
Date:   Thu Feb 18 18:53:59 2016 +0100

    Merge pull request #537 from WebAssembly/update_binary
    
    Update BinaryFormat with details about encoding

[33mcommit 413c8bc3f6ef7dccd849849aeee5fc015c63cd06[m
Merge: 8f4be3c 4b2bf18
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Feb 12 17:32:57 2016 +0100

    Merge pull request #542 from JSStats/mem-pages
    
    Initial and maximum memory size must be a multple of the page size.

[33mcommit 0df869bb0a69e16f1faf71cabdb8a7dcb15d0857[m
Author: titzer <titzer@google.com>
Date:   Thu Feb 11 15:51:50 2016 +0100

    Update compression

[33mcommit a2072993c67cb1cb773129800e50dc08ea2c3c89[m
Author: titzer <titzer@google.com>
Date:   Thu Feb 11 15:46:24 2016 +0100

    Update BinaryEncoding.md

[33mcommit 2f64313c57309d2c21ab07ef47ad7403da56fd4b[m
Author: titzer <titzer@google.com>
Date:   Thu Feb 11 15:45:44 2016 +0100

    Update BinaryEncoding.md

[33mcommit acab7ac6a768872f46d24dfeb3358804df49a403[m
Author: titzer <titzer@google.com>
Date:   Thu Feb 11 10:44:12 2016 +0100

    Correct off by one

[33mcommit 4b2bf18cf71254639a0347e1b651ae5d9107056d[m
Author: Douglas Crosher <info@jsstats.com>
Date:   Thu Feb 11 12:33:29 2016 +1100

    Initial and maximum memory size must be a multple of the page size.

[33mcommit 8f4be3cacc122014d3020986e9591251b1a54c89[m
Merge: 76e83ba ac5a591
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Feb 10 08:35:02 2016 -0800

    Merge pull request #541 from Teemperor/patch-7
    
    Added missing bracket

[33mcommit ac5a591752bceeab574f718abce903363b98e73e[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Wed Feb 10 17:32:32 2016 +0100

    Added missing bracket

[33mcommit 34165f655bdcec2e380142b82203253fe8290d20[m
Author: titzer <titzer@google.com>
Date:   Tue Feb 9 10:33:13 2016 +0100

    Update BinaryEncoding.md

[33mcommit eaf8d098eef082c5b28baacb70388f6767d8887b[m
Author: titzer <titzer@google.com>
Date:   Tue Feb 9 10:31:51 2016 +0100

    Update BinaryEncoding.md

[33mcommit 550cd2938dbc054beb099391479cfb8e8a0523e7[m
Author: titzer <titzer@google.com>
Date:   Tue Feb 9 10:30:54 2016 +0100

    Update BinaryEncoding.md

[33mcommit 01492c6dde963ac7eb6d00d8d5754de98cc2a0c1[m
Author: titzer <titzer@google.com>
Date:   Tue Feb 9 10:29:35 2016 +0100

    Update definition of pre-order encoding.

[33mcommit fd8db871fc5d2a89b0e6aa2041695f5e6f655d00[m
Author: titzer <titzer@google.com>
Date:   Tue Feb 9 10:26:33 2016 +0100

    Add definition of value_type.

[33mcommit 9c59a694bfaf116ba63312529dd5031310b2a7c3[m
Author: titzer <titzer@google.com>
Date:   Tue Feb 9 09:26:14 2016 +0100

    Update BinaryEncoding.md

[33mcommit 685111b3b7a1bf05938d5c55bc8358d483de400a[m
Author: titzer <titzer@google.com>
Date:   Mon Feb 8 19:17:38 2016 +0100

    Add links.

[33mcommit 889483aefd4a1df55373191e82e1c26ea0a5104e[m
Author: titzer <titzer@google.com>
Date:   Mon Feb 8 19:11:36 2016 +0100

    Add simple operators.

[33mcommit 5b6d8db53998f995fb92cba04b159a49dd478bb8[m
Author: titzer <titzer@google.com>
Date:   Mon Feb 8 18:43:24 2016 +0100

    Add basic operators.

[33mcommit f20b82d3ffa27dd45f0c6ab90b8e3603b6a37b6f[m
Author: titzer <titzer@google.com>
Date:   Mon Feb 8 18:27:22 2016 +0100

    Add control flow operators.

[33mcommit 6490fe86194110c8b191b2b0c1a651dad624485c[m
Author: titzer <titzer@google.com>
Date:   Mon Feb 8 18:19:30 2016 +0100

    Add first AST section.

[33mcommit adf755b3c3d86c14bd2e71969b07cc1d5a38d4e9[m
Author: titzer <titzer@google.com>
Date:   Mon Feb 8 18:10:11 2016 +0100

    Finish adding sections

[33mcommit 5ea72fdb7fdff32ccfc47c6bb05ecfd0683e440f[m
Author: titzer <titzer@google.com>
Date:   Mon Feb 8 16:57:32 2016 +0100

    Add tables describing the encoding of different sections.

[33mcommit 76e83ba101ad4ae5857e19b55859745ef596f96b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Feb 5 15:14:25 2016 -0800

    Fix an obvious missing word.

[33mcommit 80e2d4177bed33f337b2ecfba6f599d453507338[m
Merge: 1e68271 3b31dbc
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Feb 5 10:44:47 2016 -0600

    Merge pull request #532 from WebAssembly/dynamic-linking
    
    Move dynamic linking from FutureFeatures.md to PostMVP.md

[33mcommit 1e68271809c9395c1a1832f9d5ccb0db2c42e757[m
Merge: a72a05c a6fb883
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Feb 5 11:56:33 2016 +0100

    Merge pull request #536 from haxxpop/master
    
    Change start to receive any function index to be its start function

[33mcommit a6fb8831c30753206ff068eca8643be63b42e6d3[m
Author: suphanat <haxx.pop@gmail.com>
Date:   Fri Feb 5 17:54:12 2016 +0700

    Change <int> to 42

[33mcommit 71683f2c83eb27df8a91f8b98563f701a7e3208d[m
Author: suphanat <haxx.pop@gmail.com>
Date:   Fri Feb 5 17:33:07 2016 +0700

    make <int> appear

[33mcommit 0869580c8b566aea03d709e30dddd31fdad45973[m
Author: suphanat <haxx.pop@gmail.com>
Date:   Fri Feb 5 17:16:51 2016 +0700

    Change start to receive any function index to be its start function

[33mcommit 3b31dbcd3ccd0af1c5e958a75e5e5a16da34bcca[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Jan 29 18:27:48 2016 -0600

    Move dynamic linking from FutureFeatures.md to PostMVP.md to reflect actual priority

[33mcommit a72a05c29f2a989e5d05a0214c9ad1f6d52412c5[m
Merge: 9ece612 c61a20d
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jan 29 08:26:48 2016 -0600

    Merge pull request #530 from WebAssembly/comma-conditional
    
    Remove `comma` and `conditional`

[33mcommit c61a20dc7d60f82d4ba005a2412eabd7cc088eb1[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jan 28 11:08:53 2016 -0800

    Remove more references to `comma` and `conditional`.

[33mcommit a6b713dad23502e67c5dc8b94a7e4cf2c999128c[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jan 28 11:03:49 2016 -0800

    Remove the comma and conditional operators.
    
    Comma and conditional are no longer needed, in light of #464 removing
    the statement/expression distinction.

[33mcommit 9ece6120d34d0f5bf58e0a2dd2a3360cfbff92c4[m
Merge: fb835be e9eeffc
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jan 27 22:25:49 2016 -0800

    Merge pull request #489 from WebAssembly/select-operand-order
    
    Reorder select's operands to put the condition last.

[33mcommit 5fc165f88b6fd88b685e1cdca0360bea23526658[m[33m ([m[1;31morigin/remove_has_feature[m[33m)[m
Author: titzer <titzer@google.com>
Date:   Thu Jan 21 14:55:38 2016 +0100

    Remove has_feature

[33mcommit fb835be063c65cb279021a8d059f43431694ab71[m
Merge: c8c42d6 2e8cb36
Author: titzer <titzer@google.com>
Date:   Mon Jan 18 09:38:02 2016 +0100

    Merge pull request #516 from kg/binary-rationale
    
    Move most binary rationale into the rationale document and expand on rationales

[33mcommit e9eeffca1efc2c17f8d00264a70ae628d16cc30e[m[33m ([m[1;31morigin/select-operand-order[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Dec 2 11:11:49 2015 -0800

    Specify that branch instructions' return value operands come first.

[33mcommit 2e8cb368228077f16b9cd200b37fd5be628441f4[m
Author: Katelyn Gadd <kg@luminance.org>
Date:   Fri Jan 15 13:53:36 2016 -0800

    Move most binary rationale into the rationale document. Expand on rationales for binary and layered encoding.

[33mcommit c8c42d6b725a0ae804ce618d687eeb4180a3c6d5[m
Merge: 7b7ae95 1cb42b8
Author: titzer <titzer@google.com>
Date:   Fri Jan 15 17:54:32 2016 +0100

    Merge pull request #496 from WebAssembly/binary_format
    
    Add a proposal to BinaryEncoding.md

[33mcommit 7b7ae952bebf75f63afb45ae1d2432449c6db34c[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jan 8 08:24:01 2016 -0800

    Events: a few updates / style

[33mcommit 3de116209e0e1556caa4cfe5170ef049001ee0a7[m
Merge: c519bf6 213b0e0
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jan 8 08:20:36 2016 -0800

    Merge pull request #506 from jbondc/master
    
    Add section to find events or talks related to WebAssembly

[33mcommit c519bf601bd132fc3bf1c079c500decb1ac2427f[m
Merge: f92c671 0baed57
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jan 5 10:36:34 2016 -0800

    Merge pull request #513 from WebAssembly/jfbastien-patch-1
    
    Clarify AST semantic's goals

[33mcommit 0baed571259012a5ea42c73bcecc0cea4ed29f7d[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jan 5 10:36:15 2016 -0800

    Capitalize titles

[33mcommit f58590750a70a64c0ec68b9b5ba1a418bf99eda0[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jan 5 08:06:24 2016 -0800

    Wording fixes

[33mcommit 77945d12d6b91445b0de957158832bf2c4851edf[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jan 4 19:26:40 2016 -0800

    Address kripken's comments

[33mcommit 757972cfcc527356bc877d4ac3c1466b9744f564[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jan 4 18:11:57 2016 -0800

    Clarify AST semantic's goals

[33mcommit 54574edbeb72e32a94d73abcc039773e9eb0b39d[m[33m ([m[1;31morigin/no-cases[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jan 4 08:52:04 2016 -0800

    Remove cases from `tableswitch`.

[33mcommit 213b0e0359fcd5b0f5db95b037906cbf048cc657[m
Author: jbondc <jbondc@golnetwork.com>
Date:   Fri Dec 18 17:08:33 2015 -0500

    Add YouTube link.

[33mcommit e0a7741797341ab8bb54ad6a23d5158b9459d3e5[m
Author: jbondc <jbondc@golnetwork.com>
Date:   Fri Dec 18 12:00:30 2015 -0500

    Add link, remove location of events.

[33mcommit 9fc796d9c067fa3f37cdfb67497760d9a95c2e9c[m
Author: jbondc <jbondc@golnetwork.com>
Date:   Fri Dec 18 11:26:49 2015 -0500

    Add section to make it easier to find events or talks related to WebAssembly.

[33mcommit f92c671dd76578b8103b8ac8e4c5ba9f78a0e603[m
Merge: 4dfe76f 3143f60
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Dec 12 07:13:29 2015 +0100

    Merge pull request #495 from jcbeyler/start2
    
    Define a start method

[33mcommit 4dfe76f03e1a85b5cae9b97e6b1633f84e1b3bcd[m
Merge: cbda509 b13cb08
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Dec 12 07:12:46 2015 +0100

    Merge pull request #500 from jcbeyler/llvm
    
    Added a link to the FAQ to state why LLVM is chosen now

[33mcommit 3143f6040f0c2a2e9abff9414414de415df3d1d7[m
Author: Jean Christophe Beyler <jean.christophe.beyler@intel.com>
Date:   Mon Dec 7 18:41:19 2015 -0800

    Adding the definition of a start method that would be a top-level module
    statement.

[33mcommit b13cb087d03cd27d64512ec78370b857bca3d6cb[m
Author: Jean Christophe Beyler <jean.christophe.beyler@intel.com>
Date:   Wed Dec 9 10:55:41 2015 -0800

    Added a link to the FAQ to state why LLVM is chosen now

[33mcommit cbda5090f775df257c1f56eb5c69d2161ec71e24[m
Merge: 33e6a23 022f656
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Dec 10 10:39:23 2015 -0500

    Merge pull request #491 from WebAssembly/relax-nan-signbit
    
    Make the sign bit of a new NaN value nondeterministic.

[33mcommit 022f65682d0d909fca5ffed447773fcfdb596311[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Dec 10 07:38:41 2015 -0800

    Fix missing word.

[33mcommit 5c2b499ca16f068c7dc5d4ee6a473eabd6c0b425[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Dec 3 12:46:55 2015 -0800

    Mention why generated NaN sign bits are deterministic.

[33mcommit 33fa12b3750f1456b7160481787def3c57424331[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Dec 3 09:47:24 2015 -0800

    Make the sign bit of a new NaN value nondeterministic.
    
    x86 produces NaN values with the sign bit set to 1, and other architectures
    produce NaN values with the sign bit set to 0; wasm can model this by
    just making the sign bit nondeterminisitc. This should still permit the
    NaN-boxing technique that all this NaN bit-pattern language is intended to
    support.

[33mcommit 33e6a23ef7c98b818426b9c7ec4d356be7ad7495[m
Merge: b621477 a181137
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Tue Dec 8 18:08:51 2015 -0500

    Merge pull request #475 from WebAssembly/elaborate-structured-control-flow
    
    Clarify well-structured control flow

[33mcommit 1cb42b8065c2571c2af4b8dcaa925657071697dd[m
Author: titzer <titzer@google.com>
Date:   Tue Dec 8 17:27:56 2015 +0100

    Add auxilliary link

[33mcommit afdddccee15cda2de10d0dd7880103f55d22cb3d[m
Author: titzer <titzer@google.com>
Date:   Tue Dec 8 11:41:21 2015 +0100

    Update BinaryEncoding.md

[33mcommit a18113714608d8a1769c43d31574639e3f02f1cc[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Sun Nov 22 19:43:57 2015 -0800

    elaborate on what well-structured control flow means

[33mcommit 1bfcecb88a6aa5a8c0a95e8a1aadf20adba66486[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Dec 2 10:29:30 2015 -0800

    Reorder select's operands to put the condition last.

[33mcommit b621477e213c9ad8e276239e1f7c44aea85fe55b[m
Merge: fb900db c9d9e53
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Nov 30 12:33:57 2015 -0600

    Merge pull request #455 from WebAssembly/alias-from-js
    
    Refine linear memory aliasing

[33mcommit fb900db5159ba96dff14699204adfd27f0670c23[m
Merge: 6030db8 2061aa3
Author: Seth Thompson <s3th.thompson@gmail.com>
Date:   Wed Nov 25 23:48:57 2015 -0800

    Merge pull request #471 from s3ththompson/readme
    
    add more comprehensive overview to README.md

[33mcommit 2061aa390890f39342d404345e1c013dd5f0a0a7[m
Author: Seth Thompson <s3th.thompson@gmail.com>
Date:   Wed Nov 25 23:43:19 2015 -0800

    change wasm intro line

[33mcommit 3700cf342afa0249106a7f3f7373408ddae121f4[m
Author: Seth Thompson <s3th.thompson@gmail.com>
Date:   Fri Nov 20 22:05:54 2015 -0800

    typos

[33mcommit ec3bf332ac8bc1645ac0c275ea2b3db864a2c7cd[m
Author: Seth Thompson <s3th.thompson@gmail.com>
Date:   Fri Nov 20 21:38:34 2015 -0800

    fix list spacing

[33mcommit 811d3624c0903b2330f749d19bdbef22890f6e66[m
Author: Seth Thompson <s3th.thompson@gmail.com>
Date:   Fri Nov 20 21:31:44 2015 -0800

    add more comprehensive overview to README.md

[33mcommit 6030db815d5043dcce735bf754bafa2340c668de[m
Merge: 2412b50 a386358
Author: titzer <titzer@google.com>
Date:   Thu Nov 19 20:44:51 2015 +0100

    Merge pull request #467 from WebAssembly/add_nop
    
    Add nop

[33mcommit a386358cf238f742034ca96de427123babde5e1a[m
Author: titzer <titzer@google.com>
Date:   Thu Nov 19 20:31:02 2015 +0100

    Update AstSemantics.md

[33mcommit 75a268d5766e39c0d6a3fe75c4729f83fecd6fa7[m
Author: titzer <titzer@google.com>
Date:   Thu Nov 19 20:30:46 2015 +0100

    Update AstSemantics.md

[33mcommit 2412b502e8c5114b19916191081f73362947aef1[m
Merge: 7984ce5 6cbd52f
Author: titzer <titzer@google.com>
Date:   Thu Nov 19 17:00:34 2015 +0100

    Merge pull request #464 from WebAssembly/stmt_expr
    
    Remove the statement/expression distinction.

[33mcommit 6cbd52f64278ddb7f21acc5c8925768cbae3cb85[m
Author: titzer <titzer@google.com>
Date:   Thu Nov 19 16:59:13 2015 +0100

    Remove nop again

[33mcommit e16fe1dfac2b469e388492b2715aedef7e389c0b[m
Author: titzer <titzer@google.com>
Date:   Thu Nov 19 14:37:11 2015 +0100

    Update AstSemantics.md

[33mcommit 6dfbe28de7189a32dac6add57e3cb3f0c6af0fc5[m
Author: titzer <titzer@google.com>
Date:   Thu Nov 19 14:11:52 2015 +0100

    Update AstSemantics.md

[33mcommit 7984ce54c4bc444b2942e2b2d6de3ca93d1601c8[m
Merge: ec06eeb 149abe4
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Wed Nov 18 15:38:28 2015 -0800

    Merge pull request #463 from WebAssembly/tf-consistency
    
    Mention consistency on the web as a factor in the text format

[33mcommit 6b23f30b235b9b399b8670d6892bccd9ac2a9601[m
Author: titzer <titzer@google.com>
Date:   Wed Nov 18 16:23:32 2015 +0100

    Update AstSemantics.md

[33mcommit 86913eae5aa5c5f0f3dd78c87fc3390eda9e851b[m
Author: titzer <titzer@google.com>
Date:   Wed Nov 18 16:22:47 2015 +0100

    Update AstSemantics.md

[33mcommit 0f52554ac0bfef4af87a40b55936b39c5e3d973f[m
Author: titzer <titzer@google.com>
Date:   Wed Nov 18 16:09:52 2015 +0100

    Remove the statement/expression distinction.

[33mcommit 149abe4fb3a018bdb81645509a55a631de410b5d[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Thu Nov 12 13:51:08 2015 -0800

    mention consistency on the web as a factor in the text format, when bikeshedding

[33mcommit ec06eeb0db1ce12dc05475eb8123281e55a8c879[m
Merge: ce4dd7e 07fde4b
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Fri Nov 13 11:42:44 2015 -0800

    Merge pull request #460 from WebAssembly/add-wasm-emscripten
    
    Add binaryen to list of s-expression compatible implementations

[33mcommit 07fde4b9a397963002b6a09d160532f2ec4dcd2e[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Tue Nov 10 09:42:12 2015 -0800

    add binaryen to list of s-expression compatible implementations

[33mcommit 18da0d7f629be0d95e0cb7d1053fcf1fda2dd943[m
Author: mbodart <mitch.l.bodart@intel.com>
Date:   Thu Nov 12 13:13:57 2015 -0800

    Move rotate instructions from FutureFeatures to AstSemantics.

[33mcommit ce4dd7ea362e570ae07597f000fca9ea760233a0[m
Merge: 6e88687 126b04c
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Nov 10 15:12:41 2015 -0800

    Merge pull request #459 from WebAssembly/clarify-logical
    
    Update AstSemantics.md

[33mcommit 126b04c37e8b7f790a7cf772121ad2b1f095d18f[m
Author: Brad Nelson <flagxor@gmail.com>
Date:   Tue Nov 10 14:57:17 2015 -0800

    Update AstSemantics.md

[33mcommit 6e886877664d0034048fcac2dcb88e18b768fdcf[m
Merge: 6158f35 45475bc
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Nov 9 13:33:53 2015 -0800

    Merge pull request #456 from WebAssembly/masked-shift-counts
    
    Change shift count overflow to be masking.

[33mcommit 45475bc0d33bd9d6f3cf17b7756bfa312e0c8b75[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Nov 9 13:27:09 2015 -0800

    Mention that C# has this shift behavior too.

[33mcommit c2bb26a4ab5281aad6bd3c7aab4ea730ec7cba4e[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Nov 9 12:10:01 2015 -0800

    Change shift count overflow to be masking.

[33mcommit 6158f356f0be4577ea5c226e279d7be4dbeaac88[m
Merge: 86f942e eed61a5
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Nov 9 11:15:31 2015 -0800

    Merge pull request #441 from WebAssembly/nan-propagation
    
    Full IEEE 754-2008 compliant NaN bit pattern propagation.

[33mcommit c9d9e53362e57d0879df3d6451f51f63b8cd7833[m[33m ([m[1;31morigin/alias-from-js[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Nov 9 11:59:17 2015 -0600

    Refine linear memory aliasing

[33mcommit 86f942e451a6224ce404793cc1fed959edf12ab6[m
Merge: 9f4f130 b78b7c1
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Nov 6 15:14:50 2015 -0800

    Merge pull request #444 from WebAssembly/clarify-tableswitch
    
    Revise the `tableswitch` wording.

[33mcommit 9f4f1300d48b93ba43ca2c228f8e55b271f0fddf[m
Merge: ec4d60e ed646c9
Author: Derek Schuff <dschuff@chromium.org>
Date:   Fri Nov 6 13:34:57 2015 -0800

    Merge pull request #454 from WebAssembly/unreachable
    
    Add unconditional unreachable instruction

[33mcommit ed646c9c0914adb64149b0b71e5d91f995696a22[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Fri Nov 6 10:38:52 2015 -0800

    Add unconditional unreachable instruction

[33mcommit eed61a54e9085c345395350dc7bbf9938e59b560[m[33m ([m[1;31morigin/nan-propagation[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Nov 5 13:42:47 2015 -0800

    Add a paragraph about NaN propagation rules.

[33mcommit a833d0c291335bc8f122ed0a24001c4fddfe0e56[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Nov 5 10:06:15 2015 -0800

    Add a brief mention of NaN boxing in Rationale.md.

[33mcommit e56ff62daf3464b424deac369f740e366013ebe0[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Nov 5 09:56:54 2015 -0800

    Mention that the remaining NaN nondeterminism is consistent with IEEE 754-2008.

[33mcommit 00353284e7146435a018ccbd2d37d374be98425a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Nov 5 09:54:31 2015 -0800

    Fix spello.

[33mcommit ec4d60ee891444d7dcf6766af827d425f3358f03[m
Merge: 7545973 864b345
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Nov 4 16:35:40 2015 -0800

    Merge pull request #450 from chicoxyzzy/patch-2
    
    fix spelling

[33mcommit 864b3452a57cd7556b519c64531b92dd2f164f35[m
Author: Sergey Rubanov <chi187@gmail.com>
Date:   Thu Nov 5 03:24:01 2015 +0300

    fix spelling

[33mcommit 0303ce17c22631686e78b45d191ed2ad18a17abe[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Nov 2 19:28:58 2015 -0800

    Document that the polyfill might canonicalize NaNs.

[33mcommit cfe3dad1563420718b462009ab8b7995d5942244[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 23 15:43:57 2015 -0700

    Full IEEE 754-2008 compliant NaN bit pattern propagation.
    
    The main motivation for this is to enable NaN-boxing in JITs running on
    top of WebAssembly.

[33mcommit b78b7c1a18bc5b0e87e728e2bc776a6146a4599a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Nov 3 11:41:22 2015 -0800

    Mention that case fallthrough only happens by default. Also drop parens.

[33mcommit a02d2f49619ecae1db8a35d05114a48a73a639d1[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Nov 3 11:41:03 2015 -0800

    Clarify that `tableswitch` can only branch to immediate children.

[33mcommit 7545973d9d82b980f2d58ae3503455941dcc2cf9[m
Merge: eda1e34 d9a9f10
Author: titzer <titzer@google.com>
Date:   Tue Nov 3 11:06:20 2015 -0800

    Merge pull request #442 from WebAssembly/page_size
    
    Fix the WebAssembly page size at 64KiB.

[33mcommit d9a9f101c4eb7e47ee69a13831f3023d5591142a[m[33m ([m[1;31morigin/page_size[m[33m)[m
Author: titzer <titzer@google.com>
Date:   Tue Nov 3 10:53:49 2015 -0800

    Update FutureFeatures.md

[33mcommit ac337c455b4ded617ae75cff7de7d157d95b907d[m
Author: titzer <titzer@google.com>
Date:   Tue Nov 3 10:53:16 2015 -0800

    Add link to future features.

[33mcommit 0328ca76ddb9e0a0836d131386b0b2cb717b4bf3[m
Author: titzer <titzer@google.com>
Date:   Tue Nov 3 10:52:22 2015 -0800

    Add large page section to FutureFeatures

[33mcommit 1778e2800cf7618d79b3485348ab47c51e7d8a4f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Nov 3 10:22:36 2015 -0800

    Revise the `tableswitch` wording.
    
    Say "target" to describe `tableswitch`'s table elements, to avoid the
    ambiguity of whether a `case` can be branched to from other constructs,
    and the conflict with the previous paragraph describing limitations on
    the use of labels.

[33mcommit eda1e3402346f2969e639c6b75606b169ee541f3[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Nov 3 08:44:51 2015 -0800

    Remove a word that was accidentally left behind in the revision process.

[33mcommit ae58fbaba225980d1b1777003516f47072c6ed7a[m
Merge: 542c162 60b3feb
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Nov 3 08:11:40 2015 -0800

    Merge pull request #437 from WebAssembly/control-clarify
    
    Control flow clarifications

[33mcommit 60b3febf42272d5c2a3a39129d7628a8df95bcdc[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Mon Nov 2 13:40:39 2015 -0800

    control flow clarifications

[33mcommit 5cd6b7f60aab8bc654f69b7af1c3b2cf342f58ea[m
Author: titzer <titzer@google.com>
Date:   Mon Nov 2 17:47:00 2015 -0800

    Don't repeat 64KiB

[33mcommit 1274e3dc49b58b03b7dc901975292fffb4b7f10d[m
Author: titzer <titzer@google.com>
Date:   Mon Nov 2 17:45:40 2015 -0800

    Wording cleanups

[33mcommit b890cbdf35ef3ec70f87fcb52964c2f18e6a4b3f[m
Author: Ben L. Titzer <titzer@google.com>
Date:   Mon Nov 2 16:20:25 2015 -0800

    Fix the WebAssembly page size at 64KiB.

[33mcommit 542c1623990c20c30fb363b667b48a09b5f27567[m
Merge: 294f4eb 557516e
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Nov 2 18:00:45 2015 -0600

    Merge pull request #440 from brettcannon/patch-2
    
    Fix a broken link

[33mcommit 557516e9dfcb60c41aa252caedfb015679a75254[m
Author: Brett Cannon <brettcannon@users.noreply.github.com>
Date:   Mon Nov 2 15:32:56 2015 -0800

    Fix a broken link
    
    The docs for importing is no longer in MVP.md but in Modules.md now.

[33mcommit 294f4eb2d84292ca194b7421390f3c5101fb907d[m
Merge: 80cf814 a10e484
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Nov 2 14:25:42 2015 -0800

    Merge pull request #438 from brettcannon/patch-1
    
    Remove extraneous parenthesis

[33mcommit a10e484278e0635a1cd7030ccd76d3d64de03beb[m
Author: Brett Cannon <brettcannon@users.noreply.github.com>
Date:   Mon Nov 2 14:22:53 2015 -0800

    Remove extraneous parenthesis

[33mcommit 80cf814b4bbc1b8583ba3d1f6516791a4a5a844b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Oct 27 10:16:13 2015 -0700

    Fix spelling errors.

[33mcommit eecb3d882704ffba164a3320d6c3116f39afc182[m
Merge: ca9fc75 da78aee
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Oct 30 09:00:06 2015 -0700

    Merge pull request #434 from WebAssembly/jfbastien-patch-1
    
    Tooling: JS+wasm dead code stripping

[33mcommit ca9fc753205083d9b7c75b3a4fc8fc6711f15ea6[m
Merge: 9b38ba1 74c21c8
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 30 08:33:29 2015 -0700

    Merge pull request #427 from WebAssembly/new-control-flow
    
    Flexible control-flow operators.

[33mcommit 9b38ba19ce853ba08b889477e4573cd8a6998887[m
Merge: 1855964 3fb6d47
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 30 08:31:42 2015 -0700

    Merge pull request #428 from WebAssembly/select
    
    Select operators.

[33mcommit 18559649a6dafcbf50880083a7981c383f429412[m
Merge: 6c494bc f99c9fa
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 29 23:40:15 2015 -0700

    Merge pull request #435 from mtrofin/reformat
    
    Reformat feature test scenarios

[33mcommit da78aeed9af20cbb1d24c0bc93d73d21154dd693[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 29 16:18:44 2015 -0700

    Update wording to follow surrounding code

[33mcommit f99c9fad8a23c9fc09f8ce0ba1d43ad9c2b27aa8[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Thu Oct 29 14:35:19 2015 -0700

    Reformat

[33mcommit 74e61ab2d01242762766dc6376683ff9627239fb[m
Merge: 4b001d8 6c494bc
Author: Mircea Trofin <mtrofin@google.com>
Date:   Thu Oct 29 14:27:55 2015 -0700

    Merge remote-tracking branch 'WebAssembly/master' into reformat

[33mcommit f23a77f0820aec66dd6742b78f3003b5a53c5f07[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 29 10:20:14 2015 -0700

    Tooling: JS+wasm dead code stripping
    
    As discussed in #230.

[33mcommit 6c494bc8bd0a95b5b7e6f54fc5919d2372e624cc[m
Merge: 5b2794f ac4cd1a
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Oct 28 20:40:20 2015 -0700

    Merge pull request #431 from WebAssembly/rm-max-heap
    
    Change linear memory max to be a hint

[33mcommit ac4cd1a5fe2847ef1f7ca70913fd93fb68e113ec[m[33m ([m[1;31morigin/rm-max-heap[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Oct 28 16:21:44 2015 -0500

    Add rationale

[33mcommit 5b2794f9dba66c912ed78ab9fb2758ab80c3568b[m
Merge: 6ed8e67 a971da6
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Oct 28 14:06:26 2015 -0700

    Merge pull request #432 from WebAssembly/mv-section
    
    Move 'Order of evaluation' section

[33mcommit 6ed8e6706687f557cb3f27a4c99af46a55d03f70[m
Merge: 5ba9a06 244fa81
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Oct 28 14:03:31 2015 -0700

    Merge pull request #433 from WebAssembly/mv-low-mem-fault-to-future
    
    Move low-memory faulting to FutureFeatures.md

[33mcommit c6ce2481c86524650016c6bf0c58a3cdb94e14a5[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Oct 28 10:35:45 2015 -0500

    Include feedback

[33mcommit 5ba9a06c7d57c3234fb3c001087fcf2afc91ee3c[m
Merge: bcd7187 1fe7ecb
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Oct 28 07:57:07 2015 -0700

    Merge pull request #396 from WebAssembly/indirect_calls_rationa
    
    Update rationale for indirect calls

[33mcommit 244fa8179a1beb04c9128aa763880c159e2cf133[m[33m ([m[1;31morigin/mv-low-mem-fault-to-future[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Oct 28 09:20:50 2015 -0500

    Move low-memory faulting to FutureFeatures.md

[33mcommit a971da6edc29ee0d91b27c5045f23e5e44270bcb[m[33m ([m[1;31morigin/mv-section[m[33m)[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Oct 28 09:06:35 2015 -0500

    Move 'Order of evaluation' section

[33mcommit 21de225d71d3eced3fd1c099fc48a03480001a70[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Oct 27 14:28:01 2015 -0500

    Change max to a hint

[33mcommit bcd71874f255f3bf041f2dcd1a7eb882eca6de4a[m
Merge: 0bbef77 eab56c5
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Oct 27 06:09:43 2015 -1000

    Merge pull request #418 from mtrofin/patch-2
    
    Capture of scenarios motivating feature testing.

[33mcommit eab56c5d3cf70882cc7bbffeff80c7aa7fa89784[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Tue Oct 27 09:01:09 2015 -0700

    Update Rationale.md

[33mcommit b5be0bad9dbbb3affa48adcaac67c1035e2bb32c[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Mon Oct 26 22:32:50 2015 -0700

    Update Rationale.md

[33mcommit ee2413d8d7e5ad0a341c873ee2e0f17197329dce[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Mon Oct 26 19:32:24 2015 -0700

    Update Rationale.md

[33mcommit 631a2c0befed980923d81ca7051343b3653f0de9[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Mon Oct 26 18:28:44 2015 -0700

    Update FeatureTest.md

[33mcommit 36f6256cfc61db8d0cbdbd4e325239dd8e391527[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Mon Oct 26 18:26:02 2015 -0700

    Fixed small typo

[33mcommit 639a7219b4274944ab82de73ade2297a00742954[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Mon Oct 26 18:25:29 2015 -0700

    Moved Feature testing motivating scenarios
    
    Moved Feature testing motivating scenarios to Rationale.md

[33mcommit 3fb6d475e11096802685f0ee23534aaf03791a71[m[33m ([m[1;31morigin/select[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 26 17:20:42 2015 -0700

    Reword "type-parameterized operands".

[33mcommit 73140c6f767d89a5631f3a9d6053c72d2f6ced76[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 23 16:02:55 2015 -0700

    Elaborate on how `select` works.

[33mcommit 2916879ba8b38dc6da61e016405ab1105708a37a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 23 15:48:16 2015 -0700

    Select operators.

[33mcommit 74c21c8b0392c4d94eab4a28787e7325226db139[m[33m ([m[1;31morigin/new-control-flow[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 26 12:36:20 2015 -0700

    "happen from within" => "occur within"

[33mcommit 0bbef77d5789a081f2b67ede92ac4612c6a7c07a[m
Merge: 87bf5b6 1152f9e
Author: titzer <titzer@google.com>
Date:   Mon Oct 26 11:04:54 2015 -0700

    Merge pull request #271 from WebAssembly/address-subnormals-in-the-future
    
    Move handling of subnormals into FutureFeatures.

[33mcommit 87bf5b62988eb500583b61b04803264e6c796264[m
Merge: 19f8871 4cbe46b
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 15:49:17 2015 -0700

    Merge pull request #426 from WebAssembly/opcode_to_operator
    
    Rename opcode and operation to operator.

[33mcommit 2e1ccb1887852c239148bc54b3d795db48bff119[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 23 14:46:30 2015 -0700

    Flexible control-flow operators.
    
    This introduces `br`, `br_if`, and so on, while also preserving the
    high-level `if` and `if_else` operators. It also converts `switch` into
    `tableswitch` and generalizes it to support labels in enclosing scopes.

[33mcommit 19f8871d6efa4f4ce3496fc3683463587fc911fb[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Oct 23 14:56:48 2015 -0700

    Change 'local types' to 'value types'

[33mcommit 4cbe46bd0e57eaf36e7b643cd5666461d1ce60f0[m[33m ([m[1;31morigin/opcode_to_operator[m[33m)[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:39:23 2015 -0700

    Replace `operation` to `operator`

[33mcommit 069483fa0eebfbc945b3926b55e5f0aecaaaa248[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:38:09 2015 -0700

    Replace `operation` with `operator`

[33mcommit 0d8505748fc4b8b815f051e008c38266022ef82e[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:36:30 2015 -0700

    Replace `operation` with `operator`

[33mcommit 0d6012c673e7d5c7a20c3cc9e653ec08a246766d[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:35:41 2015 -0700

    Replace `operation` with `operator`

[33mcommit f79cddcea94afb27127c8588e481f523e5b36e29[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:34:29 2015 -0700

    Replace `operation` and `operator`

[33mcommit 4c0cda451072aacdf5c61dcc063bf367ffb335bf[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:32:05 2015 -0700

    Replace `operation` with `operator`

[33mcommit d52a0596b83d45106468d88947bdbbf443179160[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:30:30 2015 -0700

    Replace `operation` with `operator`

[33mcommit 6813c12d44a0a646f176f9bfa100f4822bb8a0f8[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:21:00 2015 -0700

    Replace `operation` with `operator`

[33mcommit 4d7cf2e3affddf79e293948096744dae8274c0c8[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:18:44 2015 -0700

    Replace `opcode` and `operation` to `operator.

[33mcommit c1e365087ddd30a329b09cdc43b9d3a455a417f5[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:12:11 2015 -0700

    Replace `operation` with `operator`

[33mcommit 76e43e93df7b37d410c95938c04cb7f8e4898b62[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:11:04 2015 -0700

    Replace `opcode` and `operation` with `operator`

[33mcommit afca99b81a2c4d472f4418ac1b22f1599a396651[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:09:48 2015 -0700

    Replace `opcode` and `operation` with `operator`

[33mcommit dbbd323ead4cc047c476f453473e6a0bbba61e32[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 14:05:30 2015 -0700

    Replace `opcode` and `operation` with `operator`

[33mcommit f680241dce374c1645b540507338f23d4bd82d10[m
Merge: 10ba4e8 d526cfe
Author: Ben Smith <binjimin@gmail.com>
Date:   Fri Oct 23 13:54:05 2015 -0700

    Merge pull request #425 from WebAssembly/binji-export
    
    Define export names

[33mcommit d526cfe79639b43380bacbdc1955344c880780a6[m
Author: Ben Smith <binjimin@gmail.com>
Date:   Fri Oct 23 12:00:33 2015 -0700

    Define export names

[33mcommit 1fe7ecbd65c90b74a0f158037b8e1c7dd351df9a[m[33m ([m[1;31morigin/indirect_calls_rationa[m[33m)[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 12:03:12 2015 -0700

    Update Rationale.md

[33mcommit 10ba4e8fe2c780b364e87c5d23dea464184c390f[m
Merge: e9c549f 3226e5e
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Oct 23 11:37:29 2015 -0700

    Merge pull request #423 from WebAssembly/unsigned-address-offset
    
    Specify unsigned address offsets

[33mcommit e9c549fe676396a787cea8031ffb2b0360dd1061[m
Merge: 34abf91 ae2a117
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 11:36:28 2015 -0700

    Merge pull request #424 from WebAssembly/brackets_to_slash
    
    Replace [] in operator names with / to match spec

[33mcommit 34abf913f1f7dfcc5b057791282ce29a9f3170a3[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Oct 23 11:19:21 2015 -0700

    Chromimum

[33mcommit ae2a1179a13197d819c7f68ec8477c0070a35a59[m
Author: titzer <titzer@google.com>
Date:   Fri Oct 23 10:43:33 2015 -0700

    Replace [] in operator names with / to match spec

[33mcommit 3226e5e449eb6c3dae7dbeb026b535e69c931e9c[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Oct 23 00:01:21 2015 -0700

    Specify unsigned address offsets

[33mcommit 4d692a0f06f70b0a425339a4799599097ac67d3f[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Thu Oct 22 22:48:42 2015 -0700

    Back to pronouns.

[33mcommit 5389024c047772cfe27ceb8e76f9e3d23b9240ee[m
Merge: 7382020 0eeb341
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 22 10:45:50 2015 -1000

    Merge pull request #420 from WebAssembly/jfbastien-patch-1
    
    Rename "opcode" to "operator"

[33mcommit 0eeb3414c335fbd4c01f08d257a605e9782a3229[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 22 10:08:33 2015 -1000

    Rename "opcode" to "operator"
    
    Addresses #406.

[33mcommit 7382020d735991ca2def1fa6a306185bf0369cfe[m
Merge: ffa520d af217b3
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 22 10:22:07 2015 -0700

    Merge pull request #401 from Teemperor/patch-3
    
    Added order of evaluation

[33mcommit 7347fc654d230a4887bf24db194ce57bab04b286[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Wed Oct 21 19:51:51 2015 -0700

    Incorporated feedback.
    
    - link to PostMVP.md
    - replaced pronouns with giving the user a name in each scenario, and then using the appropriate pronoun. This makes cross-referencing scenarios easier, too.

[33mcommit 22eb1e44362fd0aa5be0e1adbf1081dbd4919a10[m
Author: Mircea Trofin <mtrofin@google.com>
Date:   Wed Oct 21 09:27:40 2015 -0700

    Separate scenarios motivating feature testing
    
    Moved a few of the already described scenarios, trying to isolate the motivating problem from possible solutions.
    
    Added a development time scenario and one for restrictive target environment.

[33mcommit ffa520d6a8825ad9d2a318dc8ac9791c8d34eb8f[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:55:23 2015 -0700

    JS -> JavaScript

[33mcommit 1447963f56998f8d3ff1c8a5892845efa96d4c8a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:54:44 2015 -0700

    JS -> JavaScript

[33mcommit 0d91c19243c5a72e7d7455b4084e4dec9469c783[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:53:50 2015 -0700

    JS -> JavaScript

[33mcommit 7d0eff46f9625ae1a9ce74d229bbf0152f08d677[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:52:43 2015 -0700

    JS -> JavaScript

[33mcommit 5a5f818352808f1cba1436a0445f796d8e8693ad[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:50:31 2015 -0700

    JS -> JavaScript

[33mcommit 616014b33db2d7ba5c4858ef5434abf495fa3f36[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:42:53 2015 -0700

    Link to node.js

[33mcommit 86908f89ea254dc22dfb690fc8618eb3ac266b3e[m
Merge: df187e1 119d5b3
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:39:52 2015 -0700

    Merge pull request #410 from WebAssembly/long-double-float128
    
    Make C/C++ long double be float128

[33mcommit df187e148230d3121e5f5f558f7a298bf58c3705[m
Merge: 194c7bd 8a79116
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Oct 17 11:39:13 2015 -0700

    Merge pull request #413 from Teemperor/patch-7
    
    Defined behavior of clz/ctz

[33mcommit 194c7bdf9f968c8023563806ab7e3c21d4156505[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Oct 16 08:40:11 2015 -0500

    Replace the other use of "layer 1" with "specific layer"

[33mcommit 5970f4ea26136f052cd9ddb4a3a2997043c4439e[m
Merge: 4410a58 e7f0ba4
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Oct 16 08:30:16 2015 -0500

    Merge pull request #412 from WebAssembly/update-future-features
    
    Provide more details in FeatureTest.md

[33mcommit e7f0ba4199b7abf9380efa890a8947d6ff459b74[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 15 17:21:21 2015 -0500

    JS -> JavaScript

[33mcommit 2e1e1a478b5fea5ab73fe9369f087ebe89477e8a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 15 17:16:20 2015 -0500

    Fix wording, add link

[33mcommit 86fc34120798c3897718db7ebaeb4ef717aba336[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 15 17:11:44 2015 -0500

    Move code example out of bullet b/c markdown dies

[33mcommit 660be238f06c0cbfe38033b611d9c077f66272a2[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 15 17:03:11 2015 -0500

    Add SIMD example and more details according to comments

[33mcommit 4410a58c4413204ef6782f636c68c077b0454c4c[m
Merge: 0af5fb1 9e03093
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 15 14:59:46 2015 -0700

    Merge pull request #414 from WebAssembly/jfbastien-patch-1
    
    Nondeterminism: has_feature

[33mcommit 9e03093037d4ebbd914c02df36bc0d1dac617adb[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 15 13:53:13 2015 -0700

    Nondeterminism: has_feature

[33mcommit 9dd1af2d94aafce077f3a5dd5f7f70d0c9806711[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 15 14:39:11 2015 -0500

    Provide more details in FeatureTest.md

[33mcommit 8a79116e4cc5627346dcd728cb0517e6f3844d16[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Thu Oct 15 21:36:58 2015 +0200

    Defined behavior of clz/ctz

[33mcommit 119d5b3125cd7e20d4ec43c069b234ae0ae9b0b9[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:14:53 2015 -0700

    Describe WebAssembly's C/C++ ABI floating point types.
    
    The rationale for making C/C++ long double something other than 64-bit is:
     - there are use cases for quad-precision floating point types, and
       "long double" is a friendlier way to satisfy those use cases than
       extensions like "__float128" since "long double" has the benefit of libm
       and printf API support.
     - long double is a lost cause in terms of portable functionality anyway.
       64-bit, 80-bit, and 128-bit long double types are in use in popular
       platforms, so people who want portable results aren't using it.
     - it's also a lost cause in terms of portable performance, as it's
       already significantly slower than double on several widely popular
       platforms, so people who want portable performance already have
       motivation to avoid it.
    
    The rationale for picking quad-precision over double-double is that even
    though double-double can be significantly faster, it has surprising
    numeric properties which make it undesirable to impose on unsuspecting
    code.

[33mcommit 0af5fb141fd794b63faac14d45371b2ea137d8c5[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 15 08:46:59 2015 -0500

    s/contract/contrast/

[33mcommit 71cfc7529c27cd38083e53189a62b25d7cc83423[m
Merge: 7896ac9 9f8fe32
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Oct 14 23:13:22 2015 -0700

    Merge pull request #411 from ckknight/patch-1
    
    Fix typo of 'simimilar' to 'similar'

[33mcommit 9f8fe323ec5958586c626d099c14d695796e0001[m
Author: Cameron Knight <ckknight@gmail.com>
Date:   Wed Oct 14 21:42:47 2015 -0700

    Fix typo of 'simimilar' to 'similar'

[33mcommit 7896ac9bc433e3cb6018d61e4ba757d2a9e3791b[m
Merge: c9844c4 5b82d7c
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Oct 14 16:46:40 2015 -0700

    Merge pull request #405 from WebAssembly/opcodes
    
    Update various references to "builtin" operations.

[33mcommit c9844c4f379d7cce88f98804ec3b71e30e2ed4a4[m
Merge: 4b001d8 3c43adf
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Oct 14 09:54:13 2015 -0700

    Merge pull request #409 from Teemperor/patch-6
    
    Fixed formatting in floating point list

[33mcommit 3c43adf1b9e544abfd454594800af7cb97b3bc41[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Wed Oct 14 18:23:01 2015 +0200

    Removed HTML

[33mcommit 2ce1121e4f1fbae948890a5403ad072725a14091[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Wed Oct 14 14:38:24 2015 +0200

    Fixed formatting (again)

[33mcommit ee5deec8a337de4659fd23c6e48819638b69a2bc[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Wed Oct 14 14:36:23 2015 +0200

    Fixed formatting

[33mcommit af217b3ac903c92b51d22c6c6ca7d9e72778617c[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Tue Oct 13 09:09:30 2015 +0200

    Formatting

[33mcommit b367c4a74281b0673dba2f50856c55768bf3df74[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Tue Oct 13 09:07:10 2015 +0200

    Update AstSemantics.md

[33mcommit 4b001d8f2b613353cf81eb24cb3bcf22724f7978[m
Merge: b386045 fece61e
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 12 21:02:15 2015 -0700

    Merge pull request #404 from WebAssembly/out-of-bounds
    
    Update references to out-of-bounds behavior for recent changes to AstSemantics.md

[33mcommit 5b82d7cd4d0a094918c9131a04f5a15c0c499475[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 12 20:53:35 2015 -0700

    Update various references to "builtin" operations.

[33mcommit fece61e1be33f165b3e3a96f6a0e4a0135638866[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Oct 8 20:38:11 2015 -0700

    Update references to out-of-bounds behavior for recent changes to AstSemantics.md.

[33mcommit 6e0d429ee85b8bac3fc84fe25658de20452d70b4[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Mon Oct 12 20:21:58 2015 +0200

    Added Example + serialized AST

[33mcommit b386045f0291abccb9b2af0706eee60bc32cc425[m
Merge: a9ec51c aa4e317
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Oct 12 11:19:47 2015 -0700

    Merge pull request #397 from WebAssembly/update-faq
    
    Update FAQ

[33mcommit 53209163667f98831a3dd263c00cd3eae2c2bab9[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Mon Oct 12 19:00:25 2015 +0200

    Statements -> nodes

[33mcommit 4fbe971ed6337d8c67fa8711273c070cc9de8fdf[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Mon Oct 12 18:59:28 2015 +0200

    + Evaluation order is always deterministic.

[33mcommit ecc6e30d35d5ded30dc0af80e4642f6c68a3e745[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Mon Oct 12 16:59:54 2015 +0200

    Update AstSemantics.md

[33mcommit 8a96943d5d7dbc40d3be65e48c1d523a317d7f04[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Mon Oct 12 13:12:01 2015 +0200

    Update AstSemantics.md

[33mcommit 03279a8e5dd610cddfbb1cea0fd0759a573a334a[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Mon Oct 12 12:24:11 2015 +0200

    Added order of evaluation
    
    Added order of evaluation notice for arithmetic operations. All other operations are to my knowledge either unary or have an obvious ordering (e.g. `if`, `do_while`).

[33mcommit a9ec51caab2ead4d479327c5ebb2dcbd22f6639c[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 9 17:12:48 2015 -0700

    Rename one more `resize_memory` to `grow_memory`.

[33mcommit aa4e31705d831945a7b61d588fd64af717cb47a9[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Oct 9 15:55:04 2015 -0700

    Update FAQ
    
    I was going to move some of it to rationale.md, but I figure I should format things similarly, and update some of what's now outdated first.

[33mcommit 5461b4a8692ee3f6534598afaa36cc48825be8e9[m
Merge: 585f95e 056781e
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Oct 9 12:27:20 2015 -0700

    Merge pull request #395 from WebAssembly/rationale-intro
    
    Rationale intro.

[33mcommit 056781e45e87b2dab02b142b62d6352f350a0075[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Oct 9 09:52:07 2015 -0700

    Simplify as suggested by luke.

[33mcommit e72e43bfac9a39a345f678321833415cd090d5f1[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Oct 9 09:12:49 2015 -0700

    Address luke's / alon's comments.

[33mcommit ab54ccefbb771fb09352cd644cc0ac6d6d5f40a2[m
Author: titzer <titzer@google.com>
Date:   Thu Oct 8 15:11:01 2015 -0700

    Update Rationale.md

[33mcommit 6d86e75f829ea02c100e6e3fd0013bff5b589d12[m
Author: titzer <titzer@google.com>
Date:   Thu Oct 8 13:50:20 2015 -0700

    Update rationale for indirect calls

[33mcommit 53b51b555c8bf85b7cfaf690e25ce159abb5e362[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 13:45:52 2015 -0700

    Rationale intro.
    
    This was slightly contended in the previous PR. Rewrite it and keep it into its own PR.

[33mcommit 585f95efe747eea81a43b57f0f20d51129117444[m
Merge: 5b5ffa4 b9bad6a
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 8 13:35:23 2015 -0700

    Merge pull request #390 from WebAssembly/rationale
    
    Add Rationale.md

[33mcommit b9bad6a333d79012cc8dfece967eb80bffb7e7d5[m
Merge: caea462 5b5ffa4
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 12:38:37 2015 -0700

    Merge.

[33mcommit caea4620a37778f8121f8a6801811ac8382cb78d[m
Merge: d5f0eeb 1d96a85
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 12:30:56 2015 -0700

    Merge branch 'rationale'

[33mcommit 1d96a85ae9e85fa6429c9cb3e57536ccafa8695c[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 12:29:30 2015 -0700

    Lower.

[33mcommit 7221b7b72386876bd529a5532235e416a1331e85[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 12:29:04 2015 -0700

    Fix copy/paste context bug.

[33mcommit 5b5ffa484574ba25ea65f35a164c20ae01b79e94[m
Merge: d5f0eeb 027ce70
Author: titzer <titzer@google.com>
Date:   Thu Oct 8 12:28:45 2015 -0700

    Merge pull request #392 from WebAssembly/add_func_ptrs
    
    Update discussion of indirect calls and function pointers

[33mcommit bd82bdef3cebc4394d00ad30ff0e93d7589d7729[m
Merge: 4d9c8a7 94638c4
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:15:13 2015 -0700

    Merge.

[33mcommit 4d9c8a72d3cbcb1c51795693a136e0bc3c4026be[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:13:41 2015 -0700

    Rebase.

[33mcommit b62e31139bfce0faf88a1b1bf46c9e1265ac953a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:10:14 2015 -0700

    Reword around coloring.

[33mcommit 5152df1ec8cd06c1fab6c163523419b44fb9c65a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:04:27 2015 -0700

    Drop shadow.

[33mcommit 62a8bb12d53f2ac2377c90efa9f76ba569a4632a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:02:42 2015 -0700

    GVN.

[33mcommit 370e16158a4b5ce06eaa38532de28714f124d918[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:54:27 2015 -0700

    Clarify address-taken locals.

[33mcommit 70daaa1877aaf15fbfd7930742774a9162811c16[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:36:56 2015 -0700

    Nit.

[33mcommit 1ab46f4f45b861b15b619738040c5e103381f25a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:35:57 2015 -0700

    Sematically useful.

[33mcommit df346221d12694e5bb76680cabf0073e8859440b[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:34:28 2015 -0700

    Drop f80, expand on HW support.

[33mcommit 38dbd0f1c12ab7be136fe76012da212781773333[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:31:20 2015 -0700

    Expand on i8/i16.

[33mcommit 38348f3ce1929c3dee15a96ab2a31a187105d8e1[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:54:43 2015 -0700

    Drop 'fairly'.

[33mcommit 935ca4ae7ba69e504fb2fa2bcd721fb895a61dcd[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:53:30 2015 -0700

    Refactor nondeterminism.

[33mcommit 68006a2e7459335e1cdd9f7418bb9e874011fc30[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:09:33 2015 -0700

    Less creative, more precise on floating-point.

[33mcommit de90fca2254ac949130faddee3ec08acc1f64c1c[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:07:05 2015 -0700

    Remove rationale intro, I'll add it back in a separate PR.

[33mcommit f1a34fbf3eee75e6bd8648a317208674d92ed0e9[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 16:29:22 2015 -0700

    Clarify varargs.

[33mcommit 6e91da7bdccad41cca45b2c3e4447ff18acd0f6b[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 16:27:15 2015 -0700

    Move some mentions of nondeterminism.

[33mcommit 14712ccd008e1d0f93cc6e1b0b4993576a6e91e4[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 16:18:04 2015 -0700

    C/C++ for address-taken.

[33mcommit 0e698ef13a2e7ca757ae609964b2d04da0017cc3[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:15:35 2015 -0700

    Clarification on being creative.

[33mcommit 659d9c9445c4baeffe6ae4330fd31589d320874f[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:12:54 2015 -0700

    Typo.

[33mcommit 7fdd2f289b79c27bc61f81d1364bb0430ced2961[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:10:16 2015 -0700

    Clarify that nondeterminism is also rationalized.

[33mcommit 694bcc5c63eb536189782b2632510ace9f649687[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:06:37 2015 -0700

    Typo.

[33mcommit 31a42501e452557b4c018e0673f8cbed5d90cffc[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:05:57 2015 -0700

    Keep details on varargs.

[33mcommit bf97af20673170ac1fd3be6b46fce640975de559[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:04:27 2015 -0700

    Leave some page_size docs in AST semantics.

[33mcommit 18b1dbb1f8c252cc2d5467028011b2452edbef1f[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:02:51 2015 -0700

    Leave some alignment docs in AST semantics.

[33mcommit 881a6cd34ff82dfbfd5935c581696d98c44ec4b3[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:00:44 2015 -0700

    Typo.

[33mcommit 0e53992bf008b3134b92ce1c30acc3ea3114c075[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:00:10 2015 -0700

    Link to types.

[33mcommit 2781f71e5871176c276ed34fe641fef71d07a281[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 13:37:05 2015 -0700

    Add Rationale.md
    
    Move some of AstSemantics.md to that document. I left things pretty much as-is for now, and will defer more contended discussions to subsequent (and more targetted) edits.

[33mcommit 94638c4e0fb322ed6344bee1bdecc39452177107[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:10:14 2015 -0700

    Reword around coloring.

[33mcommit 152cfc8a6bc5fee62f36d7ae1503124f4206394a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:04:27 2015 -0700

    Drop shadow.

[33mcommit 55d8757ab1da61c20c6e366210dc44d90a6d3190[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 11:02:42 2015 -0700

    GVN.

[33mcommit b06176c2082a649185911dbd22f79c50352a136a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:54:27 2015 -0700

    Clarify address-taken locals.

[33mcommit f488d997187186d65c2f41547ddc7920f7fbe3e1[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:36:56 2015 -0700

    Nit.

[33mcommit b92f59047e55ad54e11cdb1f866fbe05bbe46acc[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:35:57 2015 -0700

    Sematically useful.

[33mcommit 02c0c2bee2e4ca98dbabdfdc2c8c41969ddd860a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:34:28 2015 -0700

    Drop f80, expand on HW support.

[33mcommit a8a981e6c6cdbbd63713f53fec414567bc088670[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 10:31:20 2015 -0700

    Expand on i8/i16.

[33mcommit 0cf9b0e1d3b877a220875a9b94707f0a1fadc6bc[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:54:43 2015 -0700

    Drop 'fairly'.

[33mcommit 537d6e8e6663766628379c4d9713517737e9670d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:53:30 2015 -0700

    Refactor nondeterminism.

[33mcommit d5f0eeb45efad66d5408ece162a0776c3800aa71[m
Merge: f8227ec 27d27cd
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 8 12:19:06 2015 -0400

    Merge pull request #394 from WebAssembly/jfbastien-patch-1
    
    Contributing: affiliation

[33mcommit e3f7bb56e46175128f235cc9ff131ee9f05597be[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:09:33 2015 -0700

    Less creative, more precise on floating-point.

[33mcommit 05a7c611804e322156b3c5614ba75f7a16388515[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Oct 8 09:07:05 2015 -0700

    Remove rationale intro, I'll add it back in a separate PR.

[33mcommit 27d27cdb6fed5483ed72c56fd23a137fff88285a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 8 08:28:49 2015 -0700

    Contributing: affiliation
    
    We were asked to follow this affiliation rule a while ago, had been applying it, but I'd forgotten to write it down!

[33mcommit f8227ec6a139ca46f6570343cce583e4a0c51ad6[m
Merge: 02de07b 0f2f0c8
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Oct 7 15:31:42 2015 -0400

    Merge pull request #388 from WebAssembly/rename-dont-need
    
    Rename `dont_need` to `discard` and mention `decommit` as an option

[33mcommit 027ce701d7b5a467b7918d517acbf64820007afe[m
Author: titzer <titzer@google.com>
Date:   Wed Oct 7 09:24:00 2015 -0700

    Clarify relative and absolute IFT indices

[33mcommit 02de07bbfaac71b3ec34c85194af9923f924ade3[m
Merge: fdcc964 e9aefe6
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Oct 7 06:09:30 2015 -0700

    Merge pull request #393 from bnjbvr/patch-1
    
    Add a missing word in Web.md

[33mcommit e9aefe6bbcca717f8b55172acc03b5a69132f751[m
Author: Benjamin Bouvier <public@benj.me>
Date:   Wed Oct 7 12:02:10 2015 +0200

    Add a missing word in Web.md

[33mcommit 9357074551f2b74acb810f5933f010ec7548c287[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 16:29:22 2015 -0700

    Clarify varargs.

[33mcommit e2046162cecee21250612e792e07b248665c8345[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 16:27:15 2015 -0700

    Move some mentions of nondeterminism.

[33mcommit 26b5815dde448a893e15b5b93f3e777d6b9c4ab3[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 16:18:04 2015 -0700

    C/C++ for address-taken.

[33mcommit fdcc96463d63e7e6a6b14ef65ef2fb015b8d26dc[m
Merge: 3341a62 86c38e0
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Oct 6 18:40:45 2015 -0400

    Merge pull request #389 from WebAssembly/only-grow
    
    Only allow memory growth in MVP

[33mcommit 5cce2b53fae52f829daed99873b71cbe319cd097[m
Author: titzer <titzer@google.com>
Date:   Tue Oct 6 15:03:40 2015 -0700

    Update discussion of indirect calls and function pointers

[33mcommit 3341a62ba9e5d53092bfbec5520c14ff512a864f[m
Merge: 4a6b3a3 1b52521
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Oct 6 14:51:07 2015 -0700

    Merge pull request #391 from WebAssembly/typo-fix
    
    Fix a typo in the Addressing section

[33mcommit 42bf4a37677e10d18b21ad922b54592156a881ad[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:15:35 2015 -0700

    Clarification on being creative.

[33mcommit 1b525219699dd505b03f6591843cc167e3f6b849[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Tue Oct 6 14:12:00 2015 -0700

    fix a typo in Addressing section

[33mcommit 753724d626215e2162071d5b50853dce10239f20[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:12:54 2015 -0700

    Typo.

[33mcommit c44752335d6badada1be5f842dbf0bf102740660[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:10:16 2015 -0700

    Clarify that nondeterminism is also rationalized.

[33mcommit 13dd5a00457395268469cca86975818281ba5f66[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:06:37 2015 -0700

    Typo.

[33mcommit 898721a8ae6e2578e22b45cefb03136be1d279e4[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:05:57 2015 -0700

    Keep details on varargs.

[33mcommit 3eb06ed9ed9281549b62e11f6428e4710b095311[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:04:27 2015 -0700

    Leave some page_size docs in AST semantics.

[33mcommit 661ac0a3ec85a3837ff07d8b128ab37f0148d3ec[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:02:51 2015 -0700

    Leave some alignment docs in AST semantics.

[33mcommit 233619533ae3bc57aebb171461cc465c9e83a27c[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:00:44 2015 -0700

    Typo.

[33mcommit 71e29a94727e15e8aba1ccf83478f2eda260c52d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 14:00:10 2015 -0700

    Link to types.

[33mcommit a85c0fdfdd51fae382e8dfb90f19193bf437282d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Oct 6 13:37:05 2015 -0700

    Add Rationale.md
    
    Move some of AstSemantics.md to that document. I left things pretty much as-is for now, and will defer more contended discussions to subsequent (and more targetted) edits.

[33mcommit 4a6b3a3d137b402149ee12216d4c62e3d44b8ca3[m
Merge: 8994285 3683945
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Oct 6 13:22:03 2015 -0700

    Merge pull request #387 from WebAssembly/runaway-recursion
    
    Add a paragraph describing the prohibition on implicit TCO.

[33mcommit 86c38e0405769e105151dcfa09290681c47d73cc[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Oct 6 14:36:01 2015 -0500

    Only allow memory growth in MVP

[33mcommit 0f2f0c8a0d85491e3d14e20c603f5bde0ee4c00a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Oct 6 13:59:57 2015 -0500

    Verb dont_need to discard and mention decommit

[33mcommit 8994285386b76b1fd2839946effaad3a36a62037[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Oct 5 21:47:33 2015 -0500

    Update AstSemantics.md

[33mcommit 3683945d120a8a9457f66930642a0d1951e85e6f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 5 16:44:38 2015 -0700

    Reword this paragraph to avoid mentioning specific optimizations.

[33mcommit 58d223e8f650e6e75d5a4516fc6dd693d71740dd[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 5 15:53:55 2015 -0700

    Clarify that explicit tail calls would have well-defined effects on the stack.

[33mcommit c2434b1aca96d1d6de2bb7d67fd62e10e8f50d09[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 5 14:09:08 2015 -0700

    Wordsmith and add a mention of general-purpose tail calls to FutureFeatures.md

[33mcommit ee3b793143fdceb6c25cc7d5ae9ffeb95d685078[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Oct 5 13:45:10 2015 -0700

    Add a paragraph describing the prohibition on implicit TCO.
    
    This corresponds with https://github.com/WebAssembly/spec/pull/103

[33mcommit ff25acc35a19ebec5d7d1c1474e1aeea86a10622[m
Merge: 43b02ab 649529e
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Oct 2 13:10:06 2015 -0700

    Merge pull request #383 from WebAssembly/jfbastien-patch-1
    
    Slight clarification of stack exhaustion

[33mcommit 43b02ab2b406ce643d0064936f7b0a6fbd984e07[m
Merge: 944b9b6 c84c8d2
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Oct 2 10:42:32 2015 -0700

    Merge pull request #380 from WebAssembly/literal
    
    Spell out the "literal" operators.

[33mcommit 649529e02a1be18bf761f6586141e07194cf604e[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 1 14:43:57 2015 -0700

    Update Nondeterminism.md

[33mcommit 26d6f6c5325dd9e77370af915d685415db0baf50[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Oct 1 13:48:17 2015 -0700

    Slight clarification of stack exhaustion

[33mcommit 944b9b65b955a771a76b75247e5267fd2a941adf[m
Merge: b3e71bf 4c24b66
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Oct 1 12:10:53 2015 -0400

    Merge pull request #374 from WebAssembly/address-syntax
    
    Add opcode name syntax for addressing

[33mcommit c84c8d2357b95da03f71e03e12406f721e39c66b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Oct 1 07:22:31 2015 -0700

    Rename `literal` to `const`.

[33mcommit 4c24b66c68e622290314cf76f3bf513a5ce5bbd1[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 25 12:20:30 2015 -0500

    Remove mention of alignment syntax

[33mcommit ba76b25a973ca87928ca1d0838bcdfc4f264d4d8[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Sep 29 12:14:50 2015 -0700

    Say "produced" instead of "copied" to match language elsewhere in the design.

[33mcommit 1c14145d6632a7f05c189f5f76d7a42555eb5eff[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Sep 29 12:14:32 2015 -0700

    Clarify that all NaNs are supported.

[33mcommit b3e71bf410aa21faf0b3d160236e3a74436b01da[m
Merge: 85ff25b 271b78b
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 29 12:15:38 2015 -0400

    Merge pull request #373 from WebAssembly/tidy-up-memory-type
    
    Tidy up references to memory type after #366

[33mcommit b798fd505b105e418288e1f9d9ce110cc2092d57[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Sep 29 08:58:03 2015 -0700

    Spell out the "literal" operators.
    
    Also, remove a broken link to the binary encoding page. The binary
    encoding is not yet determined, but AstSemantics.md doesn't need to
    reference it right now anyway.

[33mcommit 85ff25b1eec31ae1a1d6a34d7ae0ff8dd160cd43[m
Merge: 9cf66ce edad07e
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 28 15:39:25 2015 -0700

    Merge pull request #375 from t-nelis/master
    
    Fix broken link in AST semantics

[33mcommit edad07e4577dcd2e78ca2204e95952444c460db3[m
Author: Thibault Nélis <t.nelis@csweng.com>
Date:   Mon Sep 28 21:37:01 2015 +0200

    Fix broken link in AST semantics

[33mcommit 271b78b4c7b357d4b3027c3b7dddff64b2bed31d[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 25 14:23:29 2015 -0500

    s/memory size/storage size/

[33mcommit f3e144ee808cc00fcc31953073c640bef6be826f[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 25 12:07:43 2015 -0500

    Tidy up references to memory type after #366

[33mcommit 9cf66ce7cf7504e1d79df7fe882b69a5d4ab878c[m
Merge: 66bea19 f014d3a
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Sep 24 10:56:04 2015 -0400

    Merge pull request #367 from pjuftring/master
    
    Fixes the structure of the data-section-list

[33mcommit 66bea19a1b92d0b5921915040d0396a7b602ffe1[m
Merge: f4a8ae6 23fabe4
Author: titzer <titzer@google.com>
Date:   Wed Sep 23 22:02:07 2015 +0200

    Merge pull request #366 from WebAssembly/load_store_64
    
    Add back extending load/stores for 64-bit integers.

[33mcommit f014d3ab05b726889cd9c0d02c548440b922efc4[m
Author: Patrick Uftring <pjuftring@users.noreply.github.com>
Date:   Wed Sep 23 18:47:54 2015 +0200

    Fixes the structure of the data-section-list
    
    The data-section-line was part of the code-section-list so I fixed that.
    
    I added "The generic section header" to the data-section-part, because that's what written as first entry of every other section.
    I added "(in this order)" to the data-section-part, because now the data-section has two entries.
    
    You might also want to consider unifying the indentation and symbols for nested lists, but that wouldn't make any difference to the interpretation of the markdown, so I didn't change anything there.

[33mcommit 23fabe448e9eba74ff2a5b02cac2e5f5cc5eb85f[m
Author: titzer <titzer@google.com>
Date:   Wed Sep 23 15:36:19 2015 +0200

    Update AstSemantics.md

[33mcommit 0050c509077978d81ee5740cb97664d558e8ed49[m
Author: titzer <titzer@google.com>
Date:   Wed Sep 23 15:35:27 2015 +0200

    Update AstSemantics.md

[33mcommit 3633b92bc26ed00f2dda87ea024e4e5375507847[m
Author: titzer <titzer@google.com>
Date:   Wed Sep 23 15:34:42 2015 +0200

    Update AstSemantics.md

[33mcommit ab197698b9e6bdfca62f31fab48d33f1dc3c7f25[m
Author: titzer <titzer@google.com>
Date:   Wed Sep 23 14:22:13 2015 +0200

    Update AstSemantics.md

[33mcommit f4a8ae63e9fca0306e006d9e8fcd73c647150d28[m
Merge: b1f54c2 4d7e98f
Author: Brad Nelson <flagxor@gmail.com>
Date:   Tue Sep 22 11:05:09 2015 -0700

    Merge pull request #361 from WebAssembly/jfbastien-patch-1
    
    Official text format, or lack thereof

[33mcommit b1f54c2a44a9dbe1d5a8fefc103f673288aa111d[m
Merge: f72462b 6ff2753
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Sep 22 09:34:02 2015 -0700

    Merge pull request #365 from WebAssembly/call
    
    Rename `call_direct` to `call` to match the spec repo.

[33mcommit 4d7e98f257b0af434a31ed20199e743e7ea96e0c[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 21 16:39:08 2015 -0700

    Official may be different

[33mcommit 6ff275320f85973bc7230ddbd5efc32aa8bbb42a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 21 16:26:55 2015 -0700

    Rename `call_direct` to `call` to match the spec repo.

[33mcommit f1de8db290813d7e3f1bc000f85388d563c0dab0[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 21 15:49:54 2015 -0700

    Some prototypes

[33mcommit f72462b1f3b2867870447b40857bc516d6d9173d[m
Merge: 20b21f5 8dd52ad
Author: JF Bastien <github@jfbastien.com>
Date:   Sun Sep 20 14:55:29 2015 -0700

    Merge pull request #364 from pjuftring/patch-1
    
    Corrected memory-types for i32.load-instructions

[33mcommit b8b39799663ab2ad91847022248a6650c42b8c4a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sun Sep 20 14:54:27 2015 -0700

    Avoid bikeshed

[33mcommit 8dd52ad9b0d13810157ad4be4cd204fc9cbe968a[m
Author: Patrick Uftring <pjuftring@users.noreply.github.com>
Date:   Sun Sep 20 21:11:51 2015 +0200

    Corrected memory-types for i32.load-instructions
    
    I guess someone copied the "sign/zero-extend i8 to i32" for i32.load16 but forgot to change i8 to i16 so I corrected that.
    Furthermore I changed the "uint8" to "i8", maybe you want to have a "unsigned" here, but "uint8" is definitely not a defined memory-type.

[33mcommit c8dff1d01fb3476e090e9ce18b8b27ba2fab38c6[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Sep 19 16:15:52 2015 -0700

    Address human feedback

[33mcommit 6832753bd6e2420492bc1460db31865a6b12da68[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Sep 19 12:26:44 2015 -0700

    Official text format, or lack thereof
    
    The official documents haven't really clarified what we're doing with the text format, and that leads to confusion:
    http://stackoverflow.com/questions/32666337/will-web-assembly-wasm-have-its-own-syntax/
    
    This edit mentions all the current text formats.

[33mcommit 20b21f57a978f3d753ffabad060cbe5761d5be89[m
Merge: babaf50 604d74f
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Sep 18 10:17:31 2015 -0700

    Merge pull request #359 from WebAssembly/or
    
    Rename 'ior' to 'or', to match the spec repo.

[33mcommit 604d74f92dc2a56fb9a65cdc8122ce7220dc38df[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Sep 18 06:37:02 2015 -0700

    Rename 'ior' to 'or', to match the spec repo.

[33mcommit babaf50eaed8a864f26084a7fa42065b4540d9ac[m
Merge: 47d9ffa 618fc54
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Sep 17 08:28:46 2015 -0700

    Merge pull request #296 from WebAssembly/constrain-page-size
    
    Elaborate on the semantics of `page_size`.

[33mcommit 618fc5407b49acbaa3b7f143fd799c781b490cde[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Sep 17 07:48:36 2015 -0700

    Simplify the description of page_size's type and result value.

[33mcommit 88d6193bd727f09bef72e852301ebaa95967daae[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Sep 17 07:46:23 2015 -0700

    Remove the sentance about page_size trapping.

[33mcommit 47d9ffa7bbf55f94cfe249ca1236d1c13dd52417[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Sep 16 23:25:12 2015 -0500

    Fix broken link in AstSemantics.md.

[33mcommit 7b1477ee443a7f697bcb2c1d43091e8c52ab16b2[m
Merge: 9b177b6 18d1aa3
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Sep 16 15:11:18 2015 -0700

    Merge pull request #344 from WebAssembly/rm-globals
    
    Move globals out of the MVP

[33mcommit 9b177b62e11b16369c6f06f6e2664d2c032dc52c[m
Merge: 51cdb75 8f7ce6c
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Sep 16 14:57:30 2015 -0500

    Merge pull request #357 from WebAssembly/use-short-type-names
    
    Use short type names (i32 instead of int32)

[33mcommit 8f7ce6ca3e2b38b3ba40255ce74d4f1f201e543e[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Sep 16 12:07:32 2015 -0500

    Also rename float32x4 to f32x4

[33mcommit 1a42cabb7d44fa8e35c03f01db84706871424dad[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Sep 16 11:22:19 2015 -0500

    Use short type names (i32 instead of int32)

[33mcommit 51cdb75771994fd4cacd2a48e6257d3b7e490615[m
Merge: 1f05c4c 73ef22f
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Sep 15 14:27:53 2015 -0700

    Merge pull request #354 from WebAssembly/jfbastien-patch-3
    
    Limit immediate address and alignment

[33mcommit 18d1aa32c1648d3bab2c8fb394b1da61b6edc057[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Sep 14 10:48:17 2015 -0500

    Mention debug symbol section

[33mcommit c931978df51cf3d71b7e67e90629beaf470bb0c9[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 11 17:45:53 2015 -0500

    FORTRAN isn't so emphatic these days

[33mcommit a812e9fe429e31dfd71bbabf7d7a37771fe71e8e[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 11 17:27:33 2015 -0500

    Move globals out of the MVP

[33mcommit 1f05c4cdbc0dc51952579dfbaba3a7a7ce710562[m
Merge: 07d995b ddad29d
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 15 11:06:22 2015 -0500

    Merge pull request #348 from WebAssembly/call-import
    
    Add call_import to call section

[33mcommit 73ef22ff32e6a106b100b23d7be863dd55aa5030[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Sep 15 08:56:49 2015 -0700

    "the same type as the address' index"

[33mcommit 07d995b1c3f357b21df1adebb6ba61babfdea674[m
Merge: f5da474 030db43
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Sep 15 08:34:35 2015 -0700

    Merge pull request #352 from WebAssembly/jfbastien-patch-2
    
    Mention virtual memory

[33mcommit 69e11c4f78c68f251ebba17bfce900fb37e7296d[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Sep 15 08:32:10 2015 -0700

    Largest pointer size is now the limit

[33mcommit f5da4747de50cba47f36d392508884c5b88d225a[m
Merge: 2af2647 7dfb664
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Sep 15 04:48:46 2015 -0700

    Merge pull request #351 from WebAssembly/jfbastien-patch-1
    
    Memory type nit

[33mcommit 2af2647d3fba6a827cb807eedad67ba9e36d5a1a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 22:57:23 2015 -0700

    Nit: modding period
    
    All other bullets have one.

[33mcommit 0716a334e84786faa39f656d1722a857923eac11[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 22:54:25 2015 -0700

    Limit immediate address and alignment
    
    I figure neither should be infinite!

[33mcommit 030db436eb6bc71d8754702a5a4ea0397811dbd3[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 22:39:46 2015 -0700

    Mention virtual memory
    
    I've found myself explaining this a few times, so I figure it's best to just state it.

[33mcommit 7dfb664d99b077fd67ba2ff65c2a8f560a252cfd[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 22:35:39 2015 -0700

    Memory type nit
    
    "Different superset" different from what? Just "superset" seems cleaner.

[33mcommit 84104e2f99b1d6ce35791063a6993256523f826a[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 22:33:33 2015 -0700

    Link to nondeterminism
    
    Another nit...

[33mcommit 2e03a649092fcfde49d128038436555133d1f504[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 22:31:17 2015 -0700

    Link to MDN article on ArrayBuffer
    
    I'm not a huge fan of mentioning JS-isms in AstSemantics, but this is a forward-looking statement so it's OK. I nonetheless want to make sure it's clear!

[33mcommit 9d818407d84ac5785d6ec3a5c4ea4b9f56ac4e35[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 22:28:10 2015 -0700

    JS -> JavaScript

[33mcommit ab3d648a34e21cdf75fec18a12852bb39e81b6c5[m
Merge: d5540ec 69efead
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 15:06:57 2015 -0700

    Merge pull request #347 from WebAssembly/future-features-tidiness
    
    Future features tidiness

[33mcommit 69efeadc71e72f1bf6bec81a95ff245cb0cd1782[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:53:29 2015 -0700

    Add another link to define 'floored'.

[33mcommit 40568dbd1bbf4cad93728253904cfaf2278b7a12[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:52:50 2015 -0700

    Update 'shr' to 'shr_s'.

[33mcommit 3b61d7316cc77d7f0b4acd54b2f3e8b9d4afae54[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:51:37 2015 -0700

    Add a link to define 'floored'.

[33mcommit b94b2e7ac8e6d966cd1d78a0776a6c257d502d2f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:49:38 2015 -0700

    Add links for MOR and MXOR.

[33mcommit 740046cd630100a71241bf918a0c68fd952d032e[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:11:08 2015 -0700

    Add floor-division and mor/mxor to FutureFeatures.md.
    
    Again, there's no guarantee that these will ever be standardized.

[33mcommit 0f21826465193bef512029b7b35317ed1460b064[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:11:03 2015 -0700

    Update and tidy up the FutureFeatures.md integer ops
    
    Caveat: there's no guarantee that we'll standardize any of these ops.
    
    Rename and reword following the conventions for signed/unsigned, and
    suggest that `abs` be signed.

[33mcommit d5540ec8f7a96443b65b065365820d106f8aca4e[m
Merge: ae649cd 905d6c4
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 14:22:59 2015 -0700

    Merge pull request #346 from WebAssembly/isa
    
    Say ISA instead of IR when describing WebAssembly.

[33mcommit 905d6c45bb6bed2bc6e9fa069f34c25f186cd0ea[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 14:20:18 2015 -0700

    Capitalize Instruction Set Architecture.

[33mcommit 422f03c0421e3b202294542567432bc65e8f935b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 14:08:53 2015 -0700

    Expand the ISA acronym at its first use.

[33mcommit 99ce38c2755f54096cf66d9384391ef4d6658649[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Sat Sep 5 13:07:49 2015 -0700

    Say ISA instead of IR when describing WebAssembly.

[33mcommit ddad29d8a819f1e74efc092b49bf6a1cda10a6f7[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Sep 14 15:53:43 2015 -0500

    Mention call_import

[33mcommit ae649cd0cda73201ec6e6b8e0f9eddb63f842b2b[m
Merge: 0389c3b 8f649fc
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 13:52:23 2015 -0700

    Merge pull request #336 from WebAssembly/wasm64
    
    wasm64, >4 GiB indexing, and a 64-bit lock-free guarantee

[33mcommit 8f649fc6030b6123864bb99c5539ffb2c51963a9[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 13:50:18 2015 -0700

    Update FAQ.md

[33mcommit 838126bb24cd99f1dc071d826c896f7cc0f8abd4[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 13:00:53 2015 -0700

    Add a sentance mentioning APIs for feature-testing wasm64 or wasm32.

[33mcommit 6a009383a1333c5b38d52ba114af6eddc6bca8d6[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Sep 14 12:54:50 2015 -0700

    Clarify that wasm32 vs wasm64 are just a flag in the module header.

[33mcommit 0389c3b37d591ee4d01f1e83bd5b602b843a4f83[m
Merge: 82be13a 0890341
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Sep 14 13:34:42 2015 -0500

    Merge pull request #342 from WebAssembly/add-alignment-syntax
    
    Mention opcode name syntax for alignment

[33mcommit 0890341ab5e32a430e489ecc337a6feb549efca0[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 11 14:14:47 2015 -0500

    Mention opcode name syntax for alignment

[33mcommit 82be13a5a049bc1fb4fed8e74f7d6a362a132e85[m
Merge: 3120f02 540d5b8
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Sep 14 08:46:31 2015 -0700

    Merge pull request #343 from WebAssembly/jfbastien-patch-1
    
    Clarify alignment + addressing

[33mcommit 540d5b868a8477e89494f3a502e868bfef473ea2[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Sep 11 15:09:34 2015 -0700

    Simplify sentence.

[33mcommit 3120f02b28feb93362376c480401378349793678[m
Merge: 5aa0259 ad3f4ad
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Sep 11 13:20:28 2015 -0700

    Merge pull request #339 from WebAssembly/feature-test
    
    Add feature test AST node

[33mcommit 0b59ef5aee9721b9adecb860698578ee7cc06d82[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Sep 11 13:18:46 2015 -0700

    Clarify alignment + addressing

[33mcommit 5aa0259dcfbc422d52f4aeb1d49269e1b717758c[m
Merge: d902182 bc2be3f
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 11 11:44:06 2015 -0500

    Merge pull request #341 from WebAssembly/update-pointers
    
    Refine set of load/store ops and rename

[33mcommit bc2be3f35ea1a70015320829ab833690aee59f02[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 11 11:22:28 2015 -0500

    Refine set of load/store ops and rename

[33mcommit ad3f4adc08f2f286c18690d14e2da0e6c6f3482c[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Sep 11 10:35:52 2015 -0500

    Mention purity

[33mcommit 7151f6dea2bc05c14aec6f7bb37a30609caf905a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Sep 10 20:21:48 2015 -0500

    Add feature test AST node

[33mcommit ac42e69b38c6a5b0aa2fe3ce52e2eba2e0e73ee8[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Sep 8 11:32:28 2015 -0700

    Add an FAQ about using 8 bytes for storing pointers.

[33mcommit 863d084235229da71ff165289d2d976da931598f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Sat Sep 5 15:21:26 2015 -0700

    Add an FAQ entry explaining the lack of an abstract `size_t`.

[33mcommit d902182706664be2f99cd3540a26fd1ddb3877cf[m
Merge: 1f9e748 e9f8bb4
Author: rossberg-chromium <rossberg@chromium.org>
Date:   Sat Sep 5 21:09:21 2015 +0200

    Merge pull request #332 from WebAssembly/consistent-signedness-addendum
    
    Consistent naming conventions for signedness, addendum

[33mcommit c455c03a6aeac6b2300c928a0d5adf0d86b2bb5b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Sat Sep 5 11:29:49 2015 -0700

    wasm64, >4 GiB indexing, and a 64-bit lock-free guarantee
    
    According to clang, all common 64-bit CPUs, x86-64, arm64, mips64,
    ppc64, sparcv9, and systemz (and bpf, if that counts) support a 64-bit
    integer type as "lock free". In the spirit of #327, this is a
    significant agreement among 64-bit architectures, but not 32-bit
    architectures.
    
    I propose we think about the >4GiB linear memory feature as belonging to
    a distinct "architecture" called *wasm64*, when we need to distinguish
    it from *wasm32*. This will allow us to say that wasm64 has up to 64-bit
    lock-free integers, while wasm32 only has up to 32-bit lock-free
    integers. If we add signal handling it could also let us say that wasm64
    has up to 64-bit signal-atomic integers (sig_atomic_t). It would also
    make it obvious what types to use around page_size and resize_memory.
    
    Except where it makes sense to make them different, wasm32 and wasm64
    would otherwise be kept identical. In particular, wasm32 would still have
    64-bit integers.
    
    The main negative consequence of this distinction is that wasm64 code
    would not be supported on many popular 32-bit CPUs. This is unfortunate,
    but it would already be the case that code using 64-bit pointers
    wouldn't run as efficiently as code using 32-bit pointers on 32-bit
    platforms.
    
    There's a desire to leave open the possibility of having both 32-bit and
    64-bit linear memory addressing within a single instance. wasm64 could
    still be made to support mixing 64-bit indices and 32-bit indices if we
    choose, for example. We could potentially even permit wasm32 libraries
    to be linked into wasm64 applications (though there would of course be
    ABI complications at the C/C++ level, non-C/C++ code might be able to
    take advantage of this).

[33mcommit 1f9e7487e27fe8471f87e6e0bbab0449fff3badb[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Sep 4 19:38:34 2015 -0700

    Fix punctuation.

[33mcommit 17a0a2c8f16956f316431171a64c4213c0ed2f3b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Sep 4 19:35:26 2015 -0700

    Fix whitespace.

[33mcommit e9f8bb47844cfae58b651d925f0c7317e118e59b[m
Author: rossberg-chromium <rossberg@chromium.org>
Date:   Sat Sep 5 03:28:36 2015 +0200

    Consistent naming conventions for signedness, addendum
    
    This naming would fit nicely with the convention and is more informative.

[33mcommit 76c7b2daafdb1532fb0ad261b5cc9c6178f8971d[m
Merge: b35807e a9b5eb1
Author: rossberg-chromium <rossberg@chromium.org>
Date:   Thu Sep 3 23:53:17 2015 +0200

    Merge pull request #329 from WebAssembly/consistent-signedness
    
    Consistent naming conventions for signedness

[33mcommit a9b5eb140997fe949db10019382726869bc5fbd3[m
Author: rossberg-chromium <rossberg@chromium.org>
Date:   Thu Sep 3 21:15:14 2015 +0200

    Consistent naming conventions for signedness
    
    As discussed in issue #317.

[33mcommit b35807eb0b94d5c55b935c1337a6b6bb2ad6f979[m
Merge: cc1d5dc 4eea93d
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Sep 3 11:26:36 2015 -0700

    Merge pull request #328 from WebAssembly/nearest
    
    Rename nearestint to nearest, aligning with the spec.

[33mcommit cc1d5dc0f75d1bba0dceb312837a7ee073c42091[m
Merge: f065b8d 8928237
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Sep 2 16:38:26 2015 -0700

    Merge pull request #327 from WebAssembly/jfbastien-patch-1
    
    Lock-free guarantees

[33mcommit 8928237836149733e14848326c3afdf293787e1f[m
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Sep 2 16:38:07 2015 -0700

    Update Portability.md

[33mcommit 4eea93dd4d042cf904fe5782656d5ad79b3cddcf[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Sep 2 15:35:38 2015 -0700

    Rename nearestint to nearest, aligning with the spec.
    
    See https://github.com/WebAssembly/spec/pull/38 .

[33mcommit cd8df8f991589c1dec9abc71304ccde30d74264c[m
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Sep 2 13:22:35 2015 -0700

    Lock-free guarantees
    
    Addresses #300.

[33mcommit f065b8de3bf18e8bd3e7c7e56ce71dfb7ecefa6b[m
Merge: 576ab19 dbcaa57
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Sep 2 09:42:44 2015 -0700

    Merge pull request #324 from WebAssembly/instance
    
    Say "instance" instead of "module" in a few places.

[33mcommit dbcaa57391bd3a28b818ff720549aaa8c0b8102e[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Sep 1 15:52:23 2015 -0700

    Say "instance" instead of "module" in a few places.

[33mcommit 576ab19e0d9876e8f1b1b6f9e9e61dcbb7ff063b[m
Merge: 5ee2807 6bed798
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Sep 1 14:13:32 2015 -0700

    Merge pull request #321 from WebAssembly/wasm-instance
    
    Be more explicit about module vs. instance

[33mcommit 6bed798a02d7e86144c6a36a215071aa94d8fc9f[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 1 15:22:11 2015 -0500

    Add more examples to instance list

[33mcommit 19d2c4c8ea59a6fa08c96456234267dd746251ea[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 1 14:49:02 2015 -0500

    Mention imports and exports in instance

[33mcommit 5ee2807cef634fe7139a629fd3f0a9d153c1f115[m
Merge: 5256bb1 5ac7303
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 1 14:26:40 2015 -0500

    Merge pull request #323 from WebAssembly/rm-polyfill-from-mvp
    
    Move "polyfill" support from MVP to FutureFeatures

[33mcommit 5ac7303a265d848380b1f14df104baf448e7a2b2[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 1 14:26:23 2015 -0500

    Fix typo

[33mcommit af0b75c8947e3149aaf171d183270fdeea4aac92[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 1 12:06:28 2015 -0500

    Mention the code of the module is in the instance

[33mcommit b83d92de00ca560d6d5a4786f66ccb91c760b732[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Sep 1 09:55:36 2015 -0500

    Remove polyfill support from MVP; add 'Better feature testing support' to FutureFeatures.md

[33mcommit 7ca35adcb377de11b61e403bbd304df1a63cf788[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Aug 31 22:40:15 2015 -0500

    Be more explicit about module vs. instance

[33mcommit 5256bb1573f2e70f73360c6c8bd00d1889d4ec0e[m
Merge: 0eb71eb 77d3112
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Aug 31 10:24:21 2015 -0700

    Merge pull request #318 from WebAssembly/jfbastien-patch-1
    
    Text format: not unique, but precise

[33mcommit 0eb71ebfda33cc40767f3c21875663bf0a72c3c8[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Aug 31 10:09:08 2015 -0700

    Fix one more reference to IEEE-754.

[33mcommit 77d3112d02576d58cb48c865b8498a03fabe5342[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Aug 28 12:52:13 2015 -0700

    Update TextFormat.md

[33mcommit 992f2396f294f991ce0d139b9819ea39c9050fa6[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Aug 28 10:05:29 2015 -0700

    Text format: not unique, but precise
    
    Addresses the conclusion reached in #292.

[33mcommit 82ccc154dd6f1e625303a96d3df22dc6b182fc5d[m
Merge: 10831da e6f3096
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Aug 27 14:40:19 2015 -0700

    Merge pull request #315 from WebAssembly/754-2008
    
    Reference IEEE 754-2008 specifically.

[33mcommit e6f309667265cb0457f90ec74fc958caf57993f8[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 27 11:59:52 2015 -0700

    Reference IEEE 754-2008 specifically.
    
    IEEE 754-2008 contains several useful improvements over IEEE 754-1985.
    And should there be a future IEEE 754 document, we can update the
    references at the time that we make any necessary semantic changes.

[33mcommit 10831da363f3ebef3cfeb1d775b972d50b96e9f6[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Wed Aug 26 15:05:59 2015 -0700

    Fix typo in DynamicLinking.md file name

[33mcommit ce509cdb9ec33fc439e389c82dffc1e641c1925d[m
Merge: 773dca0 a148746
Author: Derek Schuff <dschuff@chromium.org>
Date:   Wed Aug 26 14:42:18 2015 -0700

    Merge pull request #313 from dschuff/dl_separate_file
    
    Factor dynamic linking section into its own file

[33mcommit a148746c073dd1c4b00d2387f2b77743d6c27004[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Wed Aug 26 14:09:25 2015 -0700

    Remove reference to dynamic linking of modules, fix typos

[33mcommit 66605f678b59ecad6fcb2f883e1ddfea7969dba8[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Wed Aug 26 11:21:47 2015 -0700

    Factor dynamic linking section into its own file
    
    Fix up some references about dynamic linking and move it to its own file, since
    it will be expanding soon and referring to a fair number of other sections.
    Also add a little extra motivation to the introductory text.

[33mcommit 773dca0adefcdd44e6cbced6507822c1bfdb94d8[m
Merge: 5a0c997 fc93b34
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Aug 21 08:26:54 2015 -0700

    Merge pull request #265 from WebAssembly/jit-library
    
    Add a page about a JIT/Optimization library idea.

[33mcommit 5a0c997f03067721eebc5ce4999ea0b2a8726b80[m
Merge: 26a3b08 b2a5c9d
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Aug 20 19:04:41 2015 -0700

    Merge pull request #307 from WebAssembly/func-cleanup
    
    Tidy up function return types.

[33mcommit b2a5c9dbe814b11bc92d9e3510010c45e3352115[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 20 09:16:16 2015 -0700

    Tidy up function return types.
    
    This replaces the `void` type with the concept of a return type sequence
    which may be empty, and it defines *signature*.
    
    This brings in the parts of #278 not connected to function pointers.

[33mcommit 26a3b08741cba1bdc05b32b7a79d6ee62ada0c9c[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 20 08:44:18 2015 -0700

    Fix typos.

[33mcommit dccbd24b6b36c2ffad997709bc47fa37cd4e4cab[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 20 08:33:48 2015 -0700

    Fix typo.

[33mcommit b59cd90e2600607f4ba39a1abb6b519b94a90b50[m
Merge: 46326de a8f9e18
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Aug 19 18:01:24 2015 -0700

    Merge pull request #305 from naturaltransformation/patch-2
    
    Minor whitespace issue

[33mcommit a8f9e18d8e2f62dd9a9cc33e22339075bdd45c23[m
Author: George Kuan <george.kuan@intel.com>
Date:   Wed Aug 19 16:37:04 2015 -0700

    Minor whitespace issue

[33mcommit fc93b347c35666ee5334e621a076fdbdd167127b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Aug 19 15:45:43 2015 -0700

    Clarify that JIT libaries will facilitate experimentation.
    
    Particularly in the area of applications dynamically generating code,
    experimentation will allow us to determine which features are most
    appropriate.

[33mcommit dc77646577e9462e87634c38ae13eded78301189[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 14 08:03:43 2015 -0700

    Add a new page about the JIT/Optimization library concept.
    
    This also obviates the need for the floating point library function
    section.

[33mcommit 46326ded8f3c639c497eb75926994f91beb02d01[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Aug 18 15:10:43 2015 -0500

    Fill in TODO with link to loader roadmap

[33mcommit 80378e1ca825238c967b915cef3db6e6eb8ab2d6[m
Merge: 0d148a3 5d3dbeb
Author: titzer <titzer@google.com>
Date:   Tue Aug 18 18:34:07 2015 +0200

    Merge pull request #301 from WebAssembly/add_data_se
    
    Add data segments to binary format

[33mcommit 5d3dbeb2cb57eb5e086d4da3f95e22c5fa83d27c[m
Author: titzer <titzer@google.com>
Date:   Tue Aug 18 18:33:15 2015 +0200

    Update Modules.md

[33mcommit e9bee501b78442225667276c49d6fe370af18205[m
Author: titzer <titzer@google.com>
Date:   Tue Aug 18 18:31:19 2015 +0200

    Update BinaryEncoding.md

[33mcommit 0d148a33b2cf79289770b4c7f5b869955e24c258[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Aug 17 16:53:23 2015 -0500

    Remove dangling reference
    
    Should have been removed with the original PR that removed the referent.

[33mcommit fbacd0afe371d1e728024b1541c8c6213d026a17[m
Author: titzer <titzer@google.com>
Date:   Mon Aug 17 18:57:34 2015 +0200

    Add data segments to binary format
    
    Add a description of data segments, which are a way that the binary module can load initialized data into memory, similar to a .data section.

[33mcommit 21e8edcc945c4b7d9e961780606e647ff0e5f31b[m
Merge: 855e8d4 d74389f
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Aug 13 13:49:19 2015 -0700

    Merge pull request #298 from WebAssembly/point-to-spec
    
    Add a link to the spec repository.

[33mcommit d74389f95ecf97ea604cc8dd90e362ec8bea8d84[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 13 13:28:26 2015 -0700

    Add a link to the spec repository.

[33mcommit 855e8d43a87c7429df4e3c38b2267bd500c1c7db[m
Merge: 9563c50 1c32080
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Aug 13 10:50:09 2015 -0700

    Merge pull request #297 from WebAssembly/ordered-unordered
    
    Clarify the behavior of floating point operations in the presence of NaN

[33mcommit 02a3fe1e0f498b7771be564d9dacc76317361ee8[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 13 10:46:41 2015 -0700

    Drop the minimum page size.

[33mcommit 1c320806b77530851c2cdca70a1c26116a849507[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 13 10:17:44 2015 -0700

    Explain what "ordered" and "unordered" mean in floating point comparisons.

[33mcommit f866601eeae4aa7d6134fa0a9eb16e9f9e2a005d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 13 10:09:52 2015 -0700

    Add a note that page sizes may be virtual.

[33mcommit 9f89d88190b800fa0b7590dd2c3acfed502610ed[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 13 10:09:41 2015 -0700

    Clarify that page sizes are in bytes.

[33mcommit f428a6d069543a4341a87bf84f7c6263f8a91f62[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 13 09:49:25 2015 -0700

    Elaborate on the semantics of `page_size`.

[33mcommit ed7049ea9917725ec0f7b430937c620efda87439[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Aug 13 09:53:10 2015 -0700

    Clarify the behavior of floating point operations in the presence of NaN.

[33mcommit 9563c50a91ed3a33242930133c42ac5014434052[m
Merge: 87c01bd 83d1f09
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Aug 12 15:18:44 2015 -1000

    Merge pull request #288 from WebAssembly/refine-memory-allocation
    
    Clarify, rename, and FAQ memory allocation

[33mcommit 83d1f09542cf643db9bc47acddc990bd09490ea6[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Aug 12 15:17:22 2015 -1000

    Remove the useful anecdotes

[33mcommit 6f691e94f1053f0e2d5378d9b6cd756db3de4838[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Aug 12 11:36:58 2015 -1000

    Rephrase page_size as nullary operator, not global

[33mcommit fee4183555ccef8adda43d343d11de42fdbe77f4[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Thu Aug 6 08:44:20 2015 -1000

    Tweak FAQ wording

[33mcommit ea8763fe83b98e47af39d0cb2c16e5c8db9746d3[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Aug 5 13:09:25 2015 -1000

    Address first round of comments

[33mcommit 0ef4e93fe3bea4356fdee2c647b2dbc54cc2d3d0[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Aug 4 09:02:31 2015 -1000

    Clarify, rename, and FAQ memory allocation

[33mcommit 87c01bd0840c08aba4f233985194b1070660d592[m
Merge: a1daa43 c913833
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Aug 10 21:38:06 2015 -0700

    Merge pull request #47 from WebAssembly/not-equal
    
    Add not-equal comparisons.

[33mcommit a1daa4352d994ca78d5046b1ba116d467abf0c00[m
Merge: a71765b 78672a4
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Aug 7 12:18:02 2015 -1000

    Merge pull request #291 from WebAssembly/tidy-mvp
    
    Refactor MVP

[33mcommit 78672a4644855951c0931540184b5984e345b9b5[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Thu Aug 6 13:49:55 2015 -1000

    Refactor MVP

[33mcommit a71765b1bd20ce58e5e62b88fe377d3e53293425[m
Merge: 15ede79 8fc02a3
Author: titzer <titzer@google.com>
Date:   Thu Aug 6 19:22:28 2015 +0200

    Merge pull request #290 from naturaltransformation/master
    
    Minor clarification of int bit operations

[33mcommit 8fc02a3883ac7fa73b7fbb7115cb2820b45c994e[m
Author: George Kuan <george.kuan@intel.com>
Date:   Thu Aug 6 10:19:34 2015 -0700

    Minor clarification of int bit operations
    
    Clarify zero/one bit semantics especially in the immediate vicinity of the mentions of int value 0.

[33mcommit 15ede791a3ea2c46344535d884e7ba5357c89628[m
Merge: 5b02cc7 b2ea198
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Aug 5 13:11:52 2015 -1000

    Merge pull request #275 from WebAssembly/fill-in-webidl
    
    Fill in a more detailed sketch for GC/DOM/Web API integration

[33mcommit b2ea1983c0c0e9e03df96df0edc7e18beedd8229[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Aug 4 09:05:22 2015 -1000

    Drop cell details and move everything to GC.md

[33mcommit 23df753790db256dadc0337d87fb2b593133cae2[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Wed Jul 22 17:45:26 2015 -1000

    Address more comments

[33mcommit 494cabcb07655a5e1a7e5e63146e65b564cf1ce7[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Jul 21 13:03:54 2015 -1000

    Address some initial feedback

[33mcommit ac6c05f2d5172428b018c5374fd0d83da420564c[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Jul 21 10:57:54 2015 -1000

    Fill a more detailed sketch for how WebIDL could work

[33mcommit 5b02cc73d0eadbfe8612886cfc4de338e6b02ee3[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Mon Aug 3 13:45:05 2015 -1000

    Change TODO link to not break generate.py

[33mcommit b5d1641ed56fa1e0099218109988d731eb9ae1a9[m
Merge: 23d6ff9 3c5f390
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Aug 3 13:40:46 2015 -1000

    Merge pull request #270 from WebAssembly/better-explain-modules
    
    Consolidate explanation of modules into a new Modules.md and improve explanation

[33mcommit 23d6ff93b40a14bf1bac7f08c1deadffd0bb6d58[m
Merge: f74c4a8 f79a4fc
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jul 29 15:40:06 2015 -0700

    Merge pull request #283 from WebAssembly/64-bit-pointers
    
    Elaborate on how 64-bit indices might work.

[33mcommit f79a4fc01b1fed68c70f9f473dc0d088c760c00d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 28 14:09:13 2015 -0700

    Minor wording fixes.

[33mcommit 94947ab1193a3b7484c77d5ce85b6645221b80e0[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 28 13:13:39 2015 -0700

    Capture the consensus in #255 about 64-bit indices.

[33mcommit c913833289f2ce56b2078df12f275b93e7890be5[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 7 08:56:38 2015 -0700

    Add unequal comparisons.
    
    (x != y) can be synthesized as ((x == y) == 0), or !(x == y),
    depending on whether we have a unary ! operator, or we can reverse the arms
    of a select or the blocks of an if-else, but this feels needlessly
    minimalistic for such a common operation.

[33mcommit 3c5f39089dd523438a2d721a5517b9ce9d794c61[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Jul 24 10:50:02 2015 -1000

    Change 'thus' to 'and'

[33mcommit 916bd10e6ed49df5b022d49d5e87c3c525613ff7[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Jul 21 12:06:47 2015 -1000

    Address more comments

[33mcommit 64fb7d772126233e42da38bed7f0cf9e1803c99c[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Jul 17 14:46:46 2015 -1000

    Expand TODO and break runon sentence

[33mcommit bc2aaeb3b592873b55c97f0ce0787aeaeb8432b1[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Fri Jul 17 14:00:43 2015 -1000

    Address first round of comments

[33mcommit cdaae53dc098de760b2784e45a99aae07e424e37[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Thu Jul 16 15:42:16 2015 -1000

    Consolidate explanation of modules into a new Modules.md and improve explanation

[33mcommit f74c4a87968ecf73120cc4588e67c7ca06ab170f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jul 23 14:29:28 2015 -0700

    Fix typos.

[33mcommit c89cdeec7ac1b85052ee2d034ce82e333d95fb4e[m
Merge: b648176 eb440c4
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jul 23 13:37:47 2015 -0700

    Merge pull request #260 from WebAssembly/why-no-fast-math
    
    Add a FAQ explaining why there is no fast-math mode.

[33mcommit eb440c4d5dc8700eca70f70e4839b8b03cc8624a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jul 23 12:00:13 2015 -0700

    Mention that middle-end optimization can benefit from feature tests.

[33mcommit b6481762284d2a5c5ca6ec1338dfcfca523dd074[m
Merge: 21c7817 d813f29
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jul 23 09:31:53 2015 -0700

    Merge pull request #276 from WebAssembly/integer-cleanups
    
    Iterate on the integer operators.

[33mcommit d813f2989117da42438a866ff69f0bec4d1e27ee[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 21 16:14:54 2015 -0700

    Iterate on the integer operators.
    
    Clarify that signed integer operations don't have silent overflow,
    clarify the nature of signed division and remainder, mention that
    clz/ctz/popcnt are sign-agnostic, and reorganize the text a little.

[33mcommit 21c78170444378f560af261c19795c279f96e6f8[m
Merge: 0dac035 d69c8b8
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jul 23 09:20:08 2015 -0700

    Merge pull request #253 from WebAssembly/no-duff
    
    Clarify the AstSemantics.md's coverage of structured control flow

[33mcommit 0dac035b9b2985f7a36e8c1bb91368ecbad13299[m
Merge: 1469cd0 2c4e32f
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jul 22 15:35:24 2015 -0700

    Merge pull request #277 from WebAssembly/float-misc
    
    Conversions have nondeterministic NaN bits too.

[33mcommit 2c4e32f7b0401dbee80ba81780d9d8b95244c05b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 17 16:08:30 2015 -0700

    Conversions have nondeterministic NaN bits too.

[33mcommit 1469cd085ae57842f3a6921d484d8ef411c2d63e[m
Merge: 8e2bc31 6abdf78
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 20 10:38:39 2015 -0700

    Merge pull request #272 from WebAssembly/jfbastien-patch-2
    
    sign-agnostic instead of signed-less

[33mcommit 6abdf784a42895b0b6714990445828a86d7446eb[m
Author: JF Bastien <github@jfbastien.com>
Date:   Sun Jul 19 13:44:22 2015 -0700

    sign-agnostic instead of signed-less
    
    Resolve #268.

[33mcommit 021bd5681d533aba75540ecef20e68030e84dd7f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 17 19:33:04 2015 -0700

    Add code quotes.

[33mcommit 8e2bc313a302ef6f678b3850eb912005b99e39dd[m
Merge: a0d3784 7b350de
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 17 17:37:28 2015 -0700

    Merge pull request #264 from WebAssembly/jfbastien-patch-1
    
    FutureFeatures: clarify trigonometric functions

[33mcommit 1152f9eeb86b5fe443539063422b1c1709f23e0a[m[33m ([m[1;31morigin/address-subnormals-in-the-future[m[33m)[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 17 16:29:55 2015 -0700

    Move handling of subnormals into FutureFeatures.

[33mcommit a0d3784b4bed1a2431a0642a3a5051459f82e939[m
Merge: 0d19a06 ef866be
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 17 15:53:58 2015 -0700

    Merge pull request #251 from WebAssembly/no-js
    
    Clarify that WebAssembly doesn't depend on JS

[33mcommit ef866beef88618f995cbe4d8463c0b4602982f79[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 15:55:20 2015 -0700

    Say "VM" instead of "interpreter" in one more place.

[33mcommit 11f98ee7025fb8295a5ed9436fe86cf5e8eeb01d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 15:52:58 2015 -0700

    Say "VM" instead of "interpreter".

[33mcommit 745f3226bdf12a8cf293a46e742f23502a94ec09[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 15:46:30 2015 -0700

    Mention that WebAssembly is being designed to be executable independently of JS.

[33mcommit 58d19dcf38405246cd21816bc2ff9a9aa996b385[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 15:39:10 2015 -0700

    Say "on the Web" rather than "in a browser" when talking about Web use cases.

[33mcommit d69c8b87f427df540e5bd132ccc75281b09b1eca[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 7 07:43:45 2015 -0700

    Restore the sentance that constraints break and continue.
    
    This requires break and continue to target blocks in constructs they are
    nested in.

[33mcommit 4babacf73dbc29ee07c40a522090806401c2df9b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 17:07:34 2015 -0700

    Reformat and revise the rationale for structured control flow.

[33mcommit de54c4ce9aef34003acced5f234e70b323d3c5b3[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 16:56:20 2015 -0700

    Clarify that the well-structured invariant that prohibits duff's devices.

[33mcommit 059681d5dc1b8d65576735f8e4b2c2a2d4809cef[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 17 14:58:40 2015 -0700

    Don't mention feature-testing for FMA here.

[33mcommit e6349a7562c669f76324554c3f051079c5330fcf[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jul 10 12:48:00 2015 -0700

    Add a FAQ explaining why there is no fast-math mode.

[33mcommit 0d19a06e1e15803ebc729837703a364aea94992e[m
Merge: 8e45192 f8c29c3
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jul 15 10:54:52 2015 -1000

    Merge pull request #266 from WebAssembly/update-non-web
    
    Clarify wording concerning portable libraries in NonWeb.md

[33mcommit f8c29c3a4861e4c778259657930e340bb270c123[m
Author: Luke Wagner <luke@mozilla.com>
Date:   Tue Jul 14 15:50:11 2015 -1000

    Clarify wording concerning portable libraries in NonWeb.md

[33mcommit 7b350de793c4ad08c17465decf8f1d519c067906[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jul 13 17:55:49 2015 -0700

    More clarification

[33mcommit 144b94e17ab2c52be63e5f8925e1efa27cd68ef5[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jul 13 15:42:44 2015 -0700

    FutureFeatures: clarify trigonometric functions
    
    The text was missing some of the background discussions that led to moving these intrinsics to future features, especially dynamic linking.

[33mcommit 8e45192cb475deb21109e27a13fe9dcbd18d84c2[m
Merge: 830691f 60af9f2
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jul 13 08:42:52 2015 -0700

    Merge pull request #259 from WebAssembly/jfbastien-patch-1
    
    Clarify regular load/store semantics when threads are added

[33mcommit 60af9f2c17ff196e4cbe98d9667c0e38939a5ef1[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jul 13 08:42:39 2015 -0700

    Fix typo.

[33mcommit 30d496329c4617d021cf1edd447a5fce229efa68[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jul 10 10:16:39 2015 -0700

    Clarify MVP versus FutureFeatures.

[33mcommit 676c0b338a2e50e0886efecfe6ad82d46fff51fa[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jul 9 15:23:35 2015 -0700

    Clarify some more.

[33mcommit 830691f722eba19fb8206a10337d6690e0eb3315[m
Merge: fd45381 fa916bf
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jul 9 14:08:21 2015 -0700

    Merge pull request #244 from WebAssembly/integer-overflow
    
    Add a FutureFeatures section about integer overflow.

[33mcommit 76ca74a85c6b24bf488386b8db1159cabe7defe0[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jul 9 09:51:49 2015 -0700

    Clarify some more

[33mcommit 1b36d1970fdd66e2102faa36fb1f609a1b71769e[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jul 9 09:26:56 2015 -0700

    Clarify regular load/store semantics when threads are added
    
    Resolves #248.

[33mcommit fd4538172fc1f981e683cacd5c2a7a08ce64a45c[m
Merge: 01b23a0 e792b30
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jul 9 09:21:41 2015 -0700

    Merge pull request #245 from WebAssembly/types-and-linear-memory
    
    Types and linear memory

[33mcommit 01b23a00737911e34f5f64f4c003b92dae54979c[m
Merge: a28fabe a951f67
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jul 9 09:20:19 2015 -0700

    Merge pull request #258 from aardappel/master
    
    Clarified possible mobile/desktop use cases in NonWeb.md

[33mcommit a951f67018de4f42f6718fa7111cbdacf1031822[m
Author: Wouter van Oortmerssen <wvo@google.com>
Date:   Wed Jul 8 11:33:16 2015 -0700

    Clarified possible mobile/desktop use cases in NonWeb.md
    
    As discussed in:
    https://github.com/WebAssembly/design/issues/249
    NonWeb.md didn't capture the full generality of possible use cases
    for WebAssembly as a universal binary format.

[33mcommit a28fabeaf617a35e20a3102276029d16509810ec[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 7 13:59:58 2015 -0700

    Update another SIMD.js URL to point to the new location.

[33mcommit 489241d6b3e2cb4a262232912552ec3dd9e70f76[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 7 13:55:27 2015 -0700

    Update SIMD.js URL to point to the new official location.

[33mcommit e792b30feaa9b2d9fe406a47f4f8b5429388a15a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 7 07:46:50 2015 -0700

    Switch back to "local types".

[33mcommit 39587f1c2affed38c438b7b6f0a9955757615905[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 7 06:14:08 2015 -0700

    Avoid committing to a 64-bit mode.

[33mcommit 74e9a613f1766951dc9d613a148287598d85bc55[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jul 7 05:48:56 2015 -0700

    Avoid the word "return" to describe the result value of an operator.

[33mcommit 603f84c21795de1cc516a690efa781d9b3c2f2c2[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 17:27:37 2015 -0700

    Fix several link anchor names.

[33mcommit fa916bf6ab2f54f34cb5940d640e84297b43dc2b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 16:13:17 2015 -0700

    Reword to clarify the differences between returning an overflow result and trapping.

[33mcommit efb674f591da6011d3386f3dcebdbd4fe307164d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 13:50:28 2015 -0700

    Clarify that the constant offset is in bytes.

[33mcommit 57bdfbe7d8beef86bccf5af4854dfb1f6968651d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 12:24:32 2015 -0700

    Clarify that linear memory sizes are limited by available resources.

[33mcommit 9c6681191c7f98a03eb63a7c3e967536e0d864f3[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 12:21:54 2015 -0700

    Clarify that all addressing is done bytewise.
    
    And clarify the out-of-bounds condition.

[33mcommit a7f0fe780349380b2309b7713baa3bc5c1ff2d8d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 11:00:30 2015 -0700

    Put URLs in markdown links to keep the text tidy.

[33mcommit 1476df2e6dc693d61fd5107ff5d9369cd86dd35c[m
Merge: c49d569 6474938
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jul 6 10:30:56 2015 -0700

    Merge pull request #242 from WebAssembly/greater-than
    
    add greater than ops

[33mcommit f9c6ce2bdb5560b66e5481043bdf2ca4c7b1ff69[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 05:26:15 2015 -0800

    Note that stores return their value.

[33mcommit cdd89839f530eacec89adf9b0a0286d494846878[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 05:26:06 2015 -0800

    Reorganize the sections on types, linear memory, and variables.
    
    Put all the type information together in one consice section, and all the
    linear memory information together in one section.
    
    The following changes are notable:
     - rename *local types* to *basic types*.
     - introduce *expression types*
     - add `void` as an expression type
     - make *memory types* an explicit superset of basic types
     - drop the `load_mem` and `store_mem` operators, which are obsolete now
       that we have the new typed load and store operators.
     - drop the `load_mem_with_offset` and `store_mem_with_offset` operators
       too, and make the typed load and store operators have offsets now.
     - define *natural alignment*, *misaligned* access, and *aligned*
       access.
     - define the *effective address* of an access

[33mcommit b3c88cc3633a8cbcb45f052586d3c494e9bab740[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jul 6 05:23:47 2015 -0800

    Add a FutureFeatures section about integer overflow.

[33mcommit 647493839daf75747db88f34dfdcc55b17a6e02d[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Thu Jul 2 10:57:22 2015 -0700

    add greater than ops

[33mcommit c49d569f6cd268201034b66a54814139d49746bd[m
Merge: 5f700c1 50c1458
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jul 1 10:00:09 2015 -0700

    Merge pull request #240 from WebAssembly/jfbastien-patch-1
    
    Minor edits to binary encoding of global structure

[33mcommit 50c1458ea12b0b86476621e2e62ed806452a9b43[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 30 20:15:14 2015 -0700

    Minor edits to binary encoding of global structure

[33mcommit 5f700c1641e3bc2d1fb63f56e887ddd8dd8c59b4[m
Merge: 22b9b9c 90f8acd
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 30 20:10:45 2015 -0700

    Merge pull request #237 from WebAssembly/definition-section
    
    add opcode definitions section

[33mcommit 90f8acd4b876ab67a60abfa16cdf3f2342e031be[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Tue Jun 30 13:57:45 2015 -0700

    made code sections have 64 bit offsets

[33mcommit 1abb70631f16e6a2922f035730d6186db3d889a2[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Tue Jun 30 13:32:02 2015 -0700

    tiny update

[33mcommit ad4a73eddd074cec18a94bd6bc843f19504217e0[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 29 15:40:54 2015 -0700

    cleanup and clarifications

[33mcommit f638520afb6c669442caa7d788bbcdbef300edf8[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 29 13:50:38 2015 -0700

    some cleanup

[33mcommit f6625e1becab6672863034125163984f13a0c86f[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 29 09:47:23 2015 -0700

    combined tables

[33mcommit adad31c80812a5943605d3ae7aea766a90d4fd53[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 29 09:26:26 2015 -0700

    added type id to section

[33mcommit 2ebd97b2676fc036ff869a514f364437aece5a69[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 29 09:07:36 2015 -0700

    add opcode definitions section

[33mcommit 22b9b9c7ce06aeab5873974f829a4909a356179b[m
Merge: b9f51af 042ac8d
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Jun 27 08:41:36 2015 -0700

    Merge pull request #229 from WebAssembly/conversion-rounding
    
    Correct behavior of int to float conversions.

[33mcommit b9f51afa7b706c0b0021227500a3922dab133a3f[m
Merge: 8eda1f7 baa6991
Author: titzer <titzer@google.com>
Date:   Sat Jun 27 07:37:11 2015 +0200

    Merge pull request #233 from ricefield/patch-1
    
    fix typo

[33mcommit baa6991af9159b897b1f2c603ff4eab348be4a2a[m
Author: Jonathan Tien <jonathan.tien@gmail.com>
Date:   Fri Jun 26 19:50:45 2015 -0700

    fix typo

[33mcommit 042ac8d14d3c80e51fd032e706256da726adf9d2[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jun 25 12:10:23 2015 -0700

    Correct behavior of int to float conversions.
    
    IEEE 754 specifies that these operations round rather than overflow.

[33mcommit 8eda1f712ca505d86bf9b92efdf40fde41734468[m
Merge: 6af5105 1746b62
Author: titzer <titzer@google.com>
Date:   Thu Jun 25 10:29:04 2015 +0200

    Merge pull request #226 from WebAssembly/s_heap_memory
    
    Search and replace "heap" with "memory"

[33mcommit 1746b628e5c924c4f669d10b685c2167e34a4826[m
Author: titzer <titzer@google.com>
Date:   Thu Jun 25 00:49:41 2015 +0200

    Update AstSemantics.md

[33mcommit afd0b72ac3242a9aa11a22cfa5529ecd220fa01f[m
Author: titzer <titzer@google.com>
Date:   Thu Jun 25 00:48:02 2015 +0200

    Update FutureFeatures.md

[33mcommit b63f0ab09ced4b165804245ff18ccae77a697315[m
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:53:48 2015 +0200

    Update Web.md

[33mcommit e6cb238691cf6b50639242c5dd843e6e8e05763b[m
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:52:49 2015 +0200

    Update PostMVP.md

[33mcommit f9d9be1c7f3aed0b9eed4e9f45a5d216317b8515[m
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:51:17 2015 +0200

    Update MVP.md

[33mcommit f56d4c2806f47b426fd86727aa2a27ce208183ac[m
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:50:46 2015 +0200

    Update MVP.md

[33mcommit f2f59fb12b4c405f2c9b8f59940d4e251809442d[m
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:48:16 2015 +0200

    Update FutureFeatures.md

[33mcommit 5099b1bbdff806936386ae33938e29b8fad125b9[m
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:44:39 2015 +0200

    Update CAndC++.md

[33mcommit 2b3c9679080b79b9fb1475fccea0b1354c02e0ee[m
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:43:31 2015 +0200

    Rename heap to memory in AstSemantics.md

[33mcommit 6af51056daf3105b6bd7830599628ef4bb4ff50e[m
Merge: ad2c588 fa95fdd
Author: titzer <titzer@google.com>
Date:   Wed Jun 24 23:13:57 2015 +0200

    Merge pull request #200 from WebAssembly/wordsmith
    
    Update AstSemantics.md

[33mcommit ad2c588435eaed64bcdd0a3e7d101c9baccd818d[m
Merge: 7579ed4 75d7496
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 23 10:37:47 2015 -0700

    Merge pull request #217 from WebAssembly/code-of-conduct
    
    Add a code of ethics and professional conduct

[33mcommit 75d7496e47bc82e3153ea7c8906a28c21ff7c662[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 23 09:58:15 2015 -0700

    Fix email, add visibility note.

[33mcommit 77ae116ed3838540494b86444d251f4268ce368d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 23 09:56:09 2015 -0700

    Got a new W3C mailing list for chairs.

[33mcommit 0023010b829fe4bea626147cddb67861cdd985f3[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 23 09:02:28 2015 -0700

    Add link to W3C procedures, ombuds.

[33mcommit 24e18442fd3cd77702d2ee872051beadb732a104[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 23 08:34:20 2015 -0700

    Add a code of ethics and professional conduct
    
    WebAssembly is growing. There haven't been any regretable events, but it's been suggested that we make this community a welcoming one up-front by having a code of ethics and professional conduct.

[33mcommit fa95fdddf9c034235a320e8df12810af6baa747a[m
Author: titzer <titzer@google.com>
Date:   Tue Jun 23 13:08:17 2015 +0200

    Update AstSemantics.md

[33mcommit 2702652ef428433ded065e44b623cd4f016da117[m
Author: titzer <titzer@google.com>
Date:   Mon Jun 22 10:14:03 2015 +0200

    Update AstSemantics.md

[33mcommit 7579ed47fc0cdcd9fbb98e055d3fdb267f3adb93[m
Merge: 1b3f592 18c3edf
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 22 02:25:46 2015 -0500

    Merge pull request #211 from sideshowbarker/patch-1
    
    Point to Streams Standard in active development

[33mcommit 18c3edf31343c2a6a94d8eeb2391a0ddc57cbc1f[m
Author: Michael[tm] Smith <mike@w3.org>
Date:   Mon Jun 22 15:05:17 2015 +0900

    Point to Streams Standard in active development
    
    This duplicates #195 from @annevk (which couldn’t be merged since the fork it came from was afterward deleted).
    
    Signed-off-by: Michael[tm] Smith <mike@w3.org>

[33mcommit 1b3f592e039105f8bbc0be50ac72a90250a655e0[m
Merge: 9d2a0dd c03eae3
Author: JF Bastien <github@jfbastien.com>
Date:   Sat Jun 20 23:14:42 2015 -0700

    Merge pull request #206 from WebAssembly/brotli
    
    Brotli for BinaryEncoding compression

[33mcommit 9d2a0ddea60ddcb6838e316cabc55025e8e659af[m
Merge: 3cdd4cf d8f7f19
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Fri Jun 19 23:11:41 2015 -0700

    Merge pull request #209 from dxnn/patch-1
    
    Update FAQ.md

[33mcommit d8f7f19fcd8f6a1617089fe7b0efecdb19fee736[m
Author: dann toliver <gh@mrmuster.com>
Date:   Sat Jun 20 00:07:25 2015 -0400

    Update FAQ.md
    
    Two typo fixes

[33mcommit c03eae3a69b79408b2ab73e11ad0221c30f75e91[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 19 16:50:52 2015 +0200

    LZHAM

[33mcommit 3cdd4cf1942bce8c68e82582e0a6ed76464e2b94[m
Merge: 9325ce1 0f0029f
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 19 12:43:37 2015 +0200

    Merge pull request #201 from Teemperor/remove-length
    
    Remove unecessary length requirement

[33mcommit 9325ce14c1168d0e9c0c3b8c6e39afba1b11c195[m
Merge: b08a0f7 9c15de4
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 19 12:40:33 2015 +0200

    Merge pull request #186 from fitzgen/reference-source-map-rfc
    
    Reference the Source Map RFC repo from FutureFeatures.md

[33mcommit 5c9a5f980ef839003c4a67cbda07c836746c5766[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 19 12:01:07 2015 +0200

    Brotli for BinaryEncoding compression
    
    I received a suggestion to support Brotli, and the rationale is pretty solid:
    
    > The WOFF 2.0 team dropped both gzip and lzma to power their latest system with brotli. Because of that, brotli has already found its way to chrome and android. http://www.w3.org/TR/WOFF20ER/
    >
    > Brotli can compress 5-25 % more than gzip, is a little bit more efficient for small files than LZMA, and decompresses 3-5x faster than LZMA. (Brotli typically compresses less than LZMA for large files -- above 1 MB or so.)
    >
    > https://github.com/google/brotli
    >
    > https://datatracker.ietf.org/doc/draft-alakuijala-brotli/
    >
    > Once the specifics for WebAssembly are clear, we could even specialize the context map or the static dictionary for WebAssembly (or you can design WebAssembly it in a way that it takes maximum benefit of the existing brotli context modes -- there are three binary modes and an utf-8 mode).

[33mcommit b08a0f70cc15f53279f7dde1d828aca60af0efe3[m
Merge: 2de6e26 a601037
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 19 11:18:24 2015 +0200

    Merge pull request #202 from Jamesits/patch-1
    
    Fix grammer

[33mcommit 2de6e26c08bccdbdf7da50a238067a11943e9133[m
Merge: 190cb34 61a634d
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 19 11:16:38 2015 +0200

    Merge pull request #190 from WebAssembly/tail-call
    
    Improve docs on proper tail call.

[33mcommit 61a634d27912d7768002545149f4430009d4ea33[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 19 11:15:43 2015 +0200

    Fix nits

[33mcommit a601037f2dbd9ca9e3143a5b5ec0963b03250178[m
Author: James Swineson <jamesswineson@gmail.com>
Date:   Fri Jun 19 12:21:26 2015 +0800

    Fix grammer

[33mcommit 7d3a6995fae154bb95cae5e9404b5fb348a35adb[m
Author: titzer <titzer@google.com>
Date:   Thu Jun 18 19:11:25 2015 -0700

    Update AstSemantics.md

[33mcommit 0f0029f94c35eefad105e52c40f8c8835f17fb0a[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Fri Jun 19 01:08:08 2015 +0200

    Better grammar

[33mcommit 89e9d0bc043bc59fd0ec42157b33df63db9f8770[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Fri Jun 19 00:19:48 2015 +0200

    Remove unecessary length requirement

[33mcommit bddf5922159ebda67bac808c601556689d0790ad[m
Author: titzer <titzer@google.com>
Date:   Thu Jun 18 15:14:23 2015 -0700

    Update AstSemantics.md

[33mcommit 190cb34d2c1d23159c0a810b81a74e8a9053a334[m
Merge: 7716381 e3d08be
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Thu Jun 18 13:00:31 2015 -0700

    Merge pull request #194 from dschuff/faq_llvmir
    
    Add "Why not use LLVM IR" to FAQ

[33mcommit e3d08be8cc86caf51edcdabfa7eca523b986a9cf[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 12:51:58 2015 -0700

    Clarify UB as runtime unpredictability and  IR as standard format

[33mcommit 7716381adab342f5c6c0f37dd9b230bea1fa1910[m
Merge: 745c85f e6698cb
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Thu Jun 18 12:34:43 2015 -0700

    Merge pull request #197 from Teemperor/fix-w3
    
    Fixed link to w3 community

[33mcommit e6698cb40f76aa3ba5cc016ba5eb802a943a0e33[m
Author: Raphael Isemann <teemperor@googlemail.com>
Date:   Thu Jun 18 20:37:51 2015 +0200

    Fixed link to w3 community

[33mcommit 75735656794b47126748ba0b6f4dbc512910927f[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 11:33:05 2015 -0700

    Remove last sentence of LLVM description and merge paragraphs

[33mcommit f1dd19890a8389bda4f0d4b7f574748b5845b8e0[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 10:58:05 2015 -0700

    Simplify first sentence about LLVM IR

[33mcommit 72e173996c7af6838e363663f4820c00f636406c[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 10:52:40 2015 -0700

    Grammar!

[33mcommit 7bf0be6f1cd004cfef73f29c875c85b8ee02d815[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 10:50:36 2015 -0700

    fix typo in link

[33mcommit 6f6ebc8f66246459e96854053cd9e78aec7dfcf3[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 10:49:03 2015 -0700

    Add mention of UB and wordsmith a bit

[33mcommit 745c85f3a1580a71bec616508f93600ede0d0405[m
Merge: 9974cde 244d318
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 18 12:44:42 2015 -0500

    Merge pull request #192 from WebAssembly/w3c
    
    Contributing: mention W3C group

[33mcommit fdcbd3736c310d05ecbc2ba94b0725b71f1581ed[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 10:34:57 2015 -0700

    review

[33mcommit 244d318d1a5fdf881cad538a43a70e11c3a613bb[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 18 19:21:52 2015 +0200

    Typo

[33mcommit 18c9539f7ea561285c3eaa12d75e45fd686c4450[m
Author: Derek Schuff <dschuff@chromium.org>
Date:   Thu Jun 18 10:12:18 2015 -0700

    Add "Why not use LLVM IR" to FAQ

[33mcommit 55be313dfab7bd70ed7a136caa9697aefc4578a4[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 18 18:42:08 2015 +0200

    Contributing: mention W3C group

[33mcommit 8cda51d5bae4125c0cefaa3dc04d65b990c49baa[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 18 15:14:51 2015 +0200

    Improve docs on proper tail call.

[33mcommit 9c15de43cf2a903a5370f3a8ac891e9cf99a7e90[m
Author: Nick Fitzgerald <fitzgen@gmail.com>
Date:   Wed Jun 17 11:18:46 2015 -0700

    Fix style to match the rest of the document.

[33mcommit fe16b973caa0592c9140d98297369f2678d41802[m
Author: Nick Fitzgerald <fitzgen@gmail.com>
Date:   Wed Jun 17 10:37:45 2015 -0700

    Reference the Source Map RFC repo from FutureFeatures.md

[33mcommit 9974cded38e762aa6e00098a7d1e6b6f0c270ba0[m[33m ([m[1;33mtag: public-announcement[m[33m)[m
Merge: dea013a f601445
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 17 08:02:46 2015 -0700

    Merge pull request #184 from WebAssembly/conversions
    
    update conversions to use type.method

[33mcommit dea013affe9ac8067251d60202d3aa493d9edb02[m
Merge: 85e1914 f098ada
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 17 11:42:00 2015 +0200

    Merge pull request #181 from WebAssembly/tweak-var-len-ints
    
    Tweak variable-length integer section

[33mcommit 85e19144b518055d647b0982bd0860d2c5d84efd[m
Merge: 0ba905a c0a50e3
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 17 01:12:34 2015 -0500

    Merge pull request #185 from WebAssembly/compression
    
    Why? Mention compression and link to stream API

[33mcommit c0a50e3be26bd6d07e98da6225106dd5b1385b3d[m
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 17 07:48:40 2015 +0200

    Why? Mention compression and link to stream API

[33mcommit 0ba905af22298f0ecb751f6876d902abcceb486d[m
Merge: e6f3f7f 31aa630
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 17:28:38 2015 -0700

    Merge pull request #182 from WebAssembly/link-to-future-features
    
    Add a link AstSemantics.md's IEEE-754 section to FutureFeatures.md's

[33mcommit e6f3f7ff0e1dccd565227a8f3761e5f0fb571039[m
Merge: b299ab2 5cef730
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Tue Jun 16 17:11:32 2015 -0700

    Merge pull request #180 from WebAssembly/compiler-options
    
    Compiler options

[33mcommit f6014452027431dc094cd431b14316e9c2c577cd[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Tue Jun 16 16:16:49 2015 -0700

    rename conversion ops

[33mcommit 159dc46418638bdc5f7c1ae60e9fbf2fa5ffeb4a[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Tue Jun 16 15:40:47 2015 -0700

    update conversions to use the type dot

[33mcommit 5cef73092010da6ee60ecaf64cbfabc9401151e4[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Tue Jun 16 10:46:07 2015 -0700

    add some context in 'which compilers' about how the new LLVM backend can be used

[33mcommit 31aa6301da6e490c7b6ea12f0518da6da32f1320[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 13:14:24 2015 -0700

    Add a link AstSemantics.md's IEEE-754 section to FutureFeatures.md's
    
    Add a link from AstSemantics.md's IEEE-754 section to
    FutureFeatures.md's section discussing the possible plans for adding
    the remaining support. And move the bullet about not providing all
    the required operations into a toplevel paragraph, since that's
    expected to stay unchanged in any case.

[33mcommit f098ada0cddb8a206d807347afeef80d9883e51d[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 14:04:50 2015 -0500

    Tweak variable-length integer section
    
    This patch removes now-useless header and qualifies experimental results.

[33mcommit b299ab25a2fca6e0612e4bdca186fbc19f9eaf81[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 11:48:14 2015 -0700

    Use an https link instead of http.

[33mcommit f1696e1e58376b4a82423d03c953bfa888d2fdcc[m
Merge: ee73a4a b771579
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 11:02:46 2015 -0700

    Merge pull request #171 from WebAssembly/full-ieee754
    
    Add a FutureFeatures.md section discussing full IEEE-754 conformance.

[33mcommit b771579559a8d8eb5c49b0fc827d29f5e2a989e4[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 09:06:35 2015 -0700

    Add a note about subnormals and link to the github issue.

[33mcommit acc06249557b7478dc33fc52d4c32da107953991[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 08:33:23 2015 -0700

    Add an introductory sentence to the IEEE-754 section.

[33mcommit 57e6145a74667df7ba7bf42f935c9368adc84538[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 13:22:03 2015 -0700

    Clarify the status of alternate execution modes.
    
    And provide a link to background reading about numerical debugging
    techniques.

[33mcommit e4265e63e57ed18f5ebc2c1f548d40401e01c235[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 10:24:32 2015 -0700

    Add a FutureFeatures.md section discussing full IEEE-754 conformance.

[33mcommit ee73a4a0d9b515d4c08dcc9b365dc553f87a2abf[m
Merge: 61b923a 9c931ce
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 12:51:56 2015 -0500

    Merge pull request #179 from WebAssembly/drop-llvm-refs
    
    Drop references to LLVM repo

[33mcommit 61b923ab4fe548e73cfdc99dddb771339416c2e0[m
Merge: a7d83db 77f43b8
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 12:50:23 2015 -0500

    Merge pull request #178 from WebAssembly/tweak-readme
    
    Tweak README.md first sentence

[33mcommit 77f43b86d6238573a2b0f941a765dbb8c7a30fda[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 11:09:49 2015 -0500

    Tweak README.md first sentence

[33mcommit a7d83db7eafe823afb902baa1905c8f233016899[m
Merge: 4c9d845 499d1f4
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 12:46:58 2015 -0500

    Merge pull request #177 from WebAssembly/add-why-q
    
    Add Why? question to FAQ

[33mcommit 4c9d845cd23148012d304b99f198a8c3629b37ed[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 09:39:57 2015 -0700

    Use code quotes and colons consistently with AstSemantics.md.

[33mcommit 759f035f65f98e5cf6c58a2558cbf3e3f7d41b29[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 09:30:54 2015 -0700

    Code-quote fma too.

[33mcommit 05ab2fa303d26edc53f2bba793972d931829af7f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 09:28:45 2015 -0700

    Code-quote and capitalize consistently with AstSemantics.md.

[33mcommit 9c931ce61d804bcff08c915c1a9726df39d32cef[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 11:26:38 2015 -0500

    Drop references to LLVM repo

[33mcommit ea06bd083415a327085984eb78a4e59ca0e4b739[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 09:26:21 2015 -0700

    Fix spello.

[33mcommit 8f67428c6f2385f4c71ec475b6200df6f0cfe35b[m
Merge: ce48fdb 32d54bc
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 10:57:43 2015 -0500

    Merge branch 'master' of https://github.com/WebAssembly/design

[33mcommit ce48fdbb5bdb53f7cbd1bbc6324b1e62dc10e07d[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 10:56:45 2015 -0500

    Fix link

[33mcommit 32d54bc4ddd2666aa5c6407758b49dcc4fedf067[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 16 07:38:25 2015 -0700

    Minor whitespace fix.

[33mcommit 499d1f42cc5e983910a431ec524b656e92c9cd30[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 09:34:04 2015 -0500

    Take three

[33mcommit 5a446643db29fefb7aa7ef37e1e92c13576c4b8c[m
Merge: a13d152 218e414
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 16 16:31:09 2015 +0200

    Merge pull request #172 from WebAssembly/refactor-ast
    
    Refactor AST semantics

[33mcommit 218e41416a5d3923d1ccfc5aeaafeb25bdb9395d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 16 16:30:25 2015 +0200

    Recover missing colons from merge.

[33mcommit bdf16bf889043114fa496f346ebf83c11e40147b[m
Merge: c934b7f a13d152
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 16 16:28:28 2015 +0200

    Merge.

[33mcommit c1ff0aab7ab84ac0b84f6b0f1400de8e8063f6f8[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 09:26:23 2015 -0500

    Add ndash

[33mcommit 81364936bb549f477a138f76f57c38b82293a5be[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 16 00:36:52 2015 -0500

    Take two

[33mcommit a13d1529d2c2c57b62880b3ec35ab7b1f784414f[m
Merge: e2c00aa a2ca639
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 20:00:14 2015 -0700

    Merge pull request #176 from WebAssembly/integer-conversions
    
    Integer conversions

[33mcommit e2c00aaffa5b63159414a27f99ce3a6d0330c7a2[m
Merge: 5425084 f7f46e9
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 16:52:50 2015 -0700

    Merge pull request #174 from WebAssembly/exhaustion
    
    Nondeterminism: examples of exhaustion

[33mcommit 542508499c4815089556b8550fb6f7b0764c3b45[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 14:41:34 2015 -0700

    Replace "IR operations" with just "operations".
    
    WebAssembly isn't called an "IR" anywhere else and it's a little
    confusing; fortunately the sentence here works without it.

[33mcommit cfed15608dc2e09d9f24b930b504007491e73ee3[m
Merge: cf781d4 9ff302e
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 16:14:30 2015 -0700

    Merge pull request #175 from WebAssembly/memoryaccess
    
    refine memory loads/stores

[33mcommit 8225d156df279b671bc54c23ced5f8fc5ab75d8d[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 15 17:53:44 2015 -0500

    Add Why? question

[33mcommit a2ca639b344d4ba790ec686f55bce0a7ae4dec89[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 15:06:05 2015 -0700

    Subject-verb agreement.

[33mcommit d1dad64553a93884b0c9e4c09a1872bb794e8927[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 15:02:34 2015 -0700

    Re-introduce integer conversion operators.

[33mcommit 9ff302ec30e71d756942ef906c243349bd6ee000[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 15 14:38:58 2015 -0700

    more rewording so conversions are explicit

[33mcommit bbb59e5063200599eb4b88d3f0245a39724bcdf8[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 15 14:37:56 2015 -0700

    Rename truncate->wrap and reword conversions from implicit->explicit

[33mcommit cf781d468394304794e29625d8bd9ca53578c353[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 14:28:00 2015 -0700

    Operator names are lower-case now.

[33mcommit d58116f9b3fb444a66a3dbdb890a68f330df913a[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 15 11:47:56 2015 -0700

    fix typos in descriptions

[33mcommit ad4137b206ad4a1886da1b95ed8731d7453c5165[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 15 11:41:59 2015 -0700

    update memory loads/stores

[33mcommit f7f46e91912716889d0cb750bac27d6911cfcb79[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 19:46:22 2015 +0200

    Nondeterminism: examples of exhaustion
    
    Also, use the same bullet lists as in other documents.

[33mcommit 4282ddc10d510842fb369f439685df289be894d3[m
Merge: 950c551 7214129
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 10:44:02 2015 -0700

    Merge pull request #173 from WebAssembly/floating-point
    
    Don't hyphenate floating-point

[33mcommit 7214129c8c04c1dfa55dd183c736ae579cfe9767[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 19:41:47 2015 +0200

    Don't hyphenate floating-point
    
    Most of the documentation doesn't have a hyphen.

[33mcommit c934b7f0936d8bca5780428cbcca6671b7521e69[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 19:35:54 2015 +0200

    Refactor AST semantics
    
    * Use .
    * Fix some typos.
    * Some word wrap.
    * A few nits.

[33mcommit 950c5514f1100b713a4d9d2d460cf6e0f0e6aadd[m
Merge: 32f7be8 18300ab
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 09:39:56 2015 -0700

    Merge pull request #164 from WebAssembly/when-do-integer-ops-trap
    
    Describe integer operation trapping with a new rule.

[33mcommit 32f7be83278936a7dec6c49268a1e134ad428188[m
Merge: 87c59de 7dbe9b7
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 18:38:05 2015 +0200

    Merge pull request #135 from WebAssembly/64-bit-int
    
    Move 64-bit integer arithmetic to MVP.

[33mcommit 87c59de19443ef45799905bf5b547e302cea5740[m
Merge: 0f08d33 ef6799f
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 18:37:10 2015 +0200

    Merge pull request #168 from WebAssembly/urlify
    
    Tidy up URLs

[33mcommit ef6799f98620b327e7668075dd709f532e60ace2[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 10:13:35 2015 -0800

    Add trailing slashes to URLs that want it.

[33mcommit 18a5288b99a812b9237f426c1194876afa2362d4[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 14:19:03 2015 -0700

    Use https links instead of http links for sites that support it.

[33mcommit 0f08d3363616080b0adeceea87f035980b843046[m
Merge: f6ca71c 4a82555
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 18:23:52 2015 +0200

    Merge pull request #169 from WebAssembly/socket.timeout
    
    Handle socket timeout.

[33mcommit 4a825556ac793cf5b1efddf2e7221fc357b9140d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 18:23:20 2015 +0200

    Handle socket timeout.

[33mcommit f6ca71cc772c023a3bd49e725886e81bf5751e8f[m
Merge: 05e1c09 bed9aa3
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 18:22:18 2015 +0200

    Merge pull request #167 from WebAssembly/readme-no-mailinglist
    
    Also remove mailing list from README.md.

[33mcommit bed9aa35a1065d713b7faea5d23352ff982e5478[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 18:11:52 2015 +0200

    Also remove mailing list from README.md.

[33mcommit 05e1c09e4bb68ded0da5fb397d787e98f166f64b[m
Merge: 6d3ec08 2a4c348
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 18:08:29 2015 +0200

    Merge pull request #166 from WebAssembly/KILL-MAILINGLIST-WITH-FIRE
    
    Remove mailing list from Contributing.md

[33mcommit 6d3ec086138bb52f556694f6323d4ac3a6c4cec0[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 15 09:07:42 2015 -0700

    Fix spello.

[33mcommit 18300abb11d88de3f3409d092d384559bca4f05f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 15:30:13 2015 -0700

    Describe integer operation trapping with a new rule.
    
    The rule is that explicitly signed or unsigned operations trap any time
    they can't return the actual result, and signed-less operations never
    trap. There's no behavior change here because this covers exactly the
    same cases that we already had covered.
    
    The beauty of this rule is that it elides trapping for add, sub, and mul
    for now, which would add too much overhead on today's hardware, while
    leaving it in place for div and rem, where it isn't as problematic and
    where we already had it, while simultaneously leaving a nice opening for
    the possible future addition of explicitly signed and unsigned versions
    of add, sub, and mul which would trap on overflow (and overflow is
    signedness-dependent anyway).
    
    This requires describing the right shifts as signedless; fortunately
    there's already a common naming we can use, "logical" and "arithmetic".

[33mcommit 2a4c348eefce2bee6a803db0192b3153327a9b94[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 16:48:34 2015 +0200

    Remove mailing list from Contributing.md
    
    Let's just worry about communicating on IRC and github, not mailing lists too.

[33mcommit 7dbe9b761d1bbbba1cb06ff1ee601b957a99c537[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 16:38:12 2015 +0200

    64-bit has the same operations.

[33mcommit e8ec77cd5c2515e698d1462ae6eb470fb32a7a09[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 16:35:21 2015 +0200

    Merge added EssentialPostMVPFeatures.md back. Delete.

[33mcommit 495430b176ab5ffcb5c7d796d13395a575aa3350[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 16:34:53 2015 +0200

    Fix capitalization.

[33mcommit 92d6871982d235d90e846688277f5fe7c1c543a9[m
Merge: 90ce54f 204b7b8
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 16:33:16 2015 +0200

    Merge.

[33mcommit 204b7b8d399122ef12a5a1376f205195238fd295[m
Merge: 064c278 34c4d45
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 16:22:57 2015 +0200

    Merge pull request #122 from WebAssembly/lowercase-names
    
    Convert operator and type names to lowercase with underscores.

[33mcommit 064c27801bcf5c2ce2cccb3c124df4c71cdff32b[m
Merge: 19ae970 dda2791
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 11:02:06 2015 +0200

    Merge pull request #162 from WebAssembly/dont-quote-nan
    
    Dont quote nan

[33mcommit 19ae97082eaf8e0cf0a90ba70357da93f3943b98[m
Merge: f5369ab 74d325e
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 11:01:00 2015 +0200

    Merge pull request #160 from WebAssembly/js-is-not-just-browsers
    
    Clarify the context in which trapping means throwing a JS exception.

[33mcommit f5369ab803d478610fe65f8cd23369ec7ae7eae1[m
Merge: 578094c 44f1337
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 11:00:11 2015 +0200

    Merge pull request #163 from WebAssembly/refactor-ccxx-ub
    
    Move the note about C/C++ undefined behavior into CAndC++.md.

[33mcommit 578094cc9332d00153068291a15a40cf09268a2f[m
Merge: 480e3d6 a8c69c8
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 10:58:00 2015 +0200

    Merge pull request #159 from WebAssembly/c-and-c++
    
    Add a note a about C and C++ language support.

[33mcommit a8c69c8871ef56fe3fe64d70956bbadca3bd8aa4[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 10:56:52 2015 +0200

    Descriptive link names; auto-vectorization later

[33mcommit 480e3d6807903368e53816d36fbcb2f3cc00bf91[m
Merge: cc8c54f a91c717
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 10:52:08 2015 +0200

    Merge pull request #158 from WebAssembly/optimism
    
    Be more optimistic about APIs for C/C++ people

[33mcommit cc8c54f182e3adf389790ffa9b8b3fe0d9a63370[m
Merge: 5cb017e ab2c10a
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 15 10:07:29 2015 +0200

    Merge pull request #156 from WebAssembly/contributing
    
    Add Contributing.md

[33mcommit ab2c10ad55126d653c9f05ab6314d60a58e48dba[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 15 10:06:10 2015 +0200

    Remove a bunch, go towards issues instead.

[33mcommit 34c4d450071635abdf045482cd0a17267718a6d2[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 15:14:41 2015 -0700

    Convert operator and type names to lowercase with underscores.

[33mcommit dda27912931cd8abedf20959af6d81b0dd45a5a9[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 13:16:32 2015 -0700

    Reduce code quoting of NaN in Polyfill.md too.

[33mcommit 66a54a781eb3d5583647aed4c92b66fb4daedb1a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 13:13:52 2015 -0700

    Don't put code quotes around NaN.
    
    Also, spell out "negative infinity" instead of saying -infinity, so that
    it doesn't look like it needs to be in code quotes either.

[33mcommit 5cb017eff2e04daa325b6170e2bc552601b14ca6[m
Merge: ea85e91 b2c9664
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 14:28:50 2015 -0700

    Merge pull request #121 from WebAssembly/categorize-ops
    
    Categorize ops previously "under consideration"

[33mcommit b2c9664a0b92a7480deb8d105be517577fa6b3ba[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 14:26:41 2015 -0700

    Fix whitespace nit.

[33mcommit b83368d73725b77c0cf636d26ec631490c489a07[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 10:10:22 2015 -0700

    Note that the min/max here are binary operators.

[33mcommit fa7f66f0dff2bb91ec25f3fbe864d2e2e4bd17cd[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jun 11 17:03:45 2015 -0700

    Drop the rest of the ops under consideration.
    
    If we don't accept !=, we'll probably want to revisit some of these.

[33mcommit deb41fb73483c6c4b6e91d811b5f13809a0e8030[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 13:08:19 2015 -0700

    Promote new proposed operations to the MVP.
    
    This is inspired by the reasoning for adding Int64 to the MVP: the
    polyfill isn't going to be worse than what compilers would have to lower
    these things into otherwise, and having them there means a nicer path
    forward to future versions.

[33mcommit 5cbe16179caa899e50fda07e5988d2c97c951ce7[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 09:13:37 2015 -0700

    Also mention conversion from float16 to float64.

[33mcommit b12be6709e1762804793c5fd22d3468c33c40026[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 05:26:21 2015 -0700

    Mention that it might be a good idea for Abs overflow to trap.

[33mcommit 79b6d14866993c57bd4a81663bf3f8059f1ce0f6[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 05:25:22 2015 -0700

    Fold "Operations which may not be available" into other sections.
    
    And add a paragraph about 128-bit floating-point support.
    
    Also specify that FMA would be IEEE-754 or not available.

[33mcommit 1578d64ffa884dae4cb57e286e16fc1ca94311d7[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 12:39:23 2015 -0800

    Add FMA to FutureFeatures.md.

[33mcommit 071bc0fd8e9ce738f00a3f699c24460806058b38[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 12:39:21 2015 -0800

    Move floating-point libary intrinsics into FutureFeatures.md.

[33mcommit c00ddd4345019805f9c01c93c354936002e60535[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 12:39:18 2015 -0800

    Add a few more integer operations to FutureFeatures.md.

[33mcommit e53a18cde5ea35e5e65dca7d39e2c5eb3d6a0dc5[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 12:39:16 2015 -0800

    Add Bang and Neg operators under consideration.
    
    Bang is unary !
    Neg is unary -

[33mcommit 35a1f2fcfcb174f9dbe3775fc75cad140e97e55e[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 12:39:14 2015 -0800

    Recategorize the floating-point operations "under consideration".
    
    min/max - Promote these to base; they are all trivially polyfillable,
    and are useful things to have in the base platform besides.
    
    nearestInt - This is not trivially polyfillable (JavaScript's Math.round
    does tie-breaking wrong), so put it in EssentialPostMVPFeatures.md so it
    can join floor/ceil.
    
    trunc - This is not trivially polyfillable (in ES6, but not in all the
    browsers we need the polyfill to support), so put it in
    EssentialPostMVPFeatures.md so it can join floor/ceil/nearestInt in
    providing rouding to integer via all the standard roundings.
    
    minNum/maxNum - Put these in FutureFeatures.md for future consideration.
    They're not trivially polyfillable, and they're not obviously essential
    at this point, though they do have uses.

[33mcommit 3b9e3d2c99f071899c25765ba30ec36693f4a7fc[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 12:39:09 2015 -0800

    Remve MulHigh operators from consideration.
    
    One of the main uses for these is implementing division-by-constant
    optimizations, and I think we can call on JITs to do those, so we don't
    need WebAssembly produers doing them and emitting larger WebAssembly
    code. For other uses, real 64-bit integers will work fairly well when
    we get those.

[33mcommit 34988152bb79df994b035745b82736d57f31f894[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 12:39:03 2015 -0800

    Recategorize several integer operations "under consideration".
    
    ctz/clz/popcount/bswap - not nicely polyfillable (ES6 will have clz, but
    not all browsers will have that in the timeframe of the polyfill), but quite
    essential in a modern ISA. These can be done in software and
    pattern-matched, but except maybe ctz, the patterns are sufficiently gross
    that I don't expect these will be controversial.
    
    rot/min/max - nice to have, but not quite as essential, so put them in
    FutureFeatures.md.

[33mcommit 44f1337b6bcddaf42e75b07e423bf14063053d52[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 13:40:04 2015 -0700

    Move the note about C/C++ undefined behavior into CAndC++.md.
    
    The text fits better there, and this also tidies up Nondeterminism.md.

[33mcommit ea85e91c5a2f54f4943a4b92f6a75f37a9a63986[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 13:31:42 2015 -0700

    Fix spelling error.

[33mcommit 0b08741a5f4234572866b0af6a01b9f5ec8953e4[m
Merge: 9fe8d0f d2121d7
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Jun 12 15:24:37 2015 -0500

    Merge pull request #161 from WebAssembly/nondeterministic-spelling
    
    Spell "nondeterministic" without a hyphen.

[33mcommit d2121d7fed111416ce5fe7c83567b12d212de39b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 13:19:14 2015 -0700

    Spell "nondeterministic" without a hyphen.
    
    "nondeterminism" too. Spellings with and without the hyphen are in
    common use, though the hyphenated form is less common. But mainly it's
    just nice to be consistent.

[33mcommit 74d325e47ae73c42a20607c18576445cca5072fa[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 12:55:52 2015 -0700

    Clarify the context in which trapping means throwing a JS exception.
    
    JS isn't just inside browsers, after all.

[33mcommit 69d21acd1a3c63d82578efc37805ad071f146ef1[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 12:52:48 2015 -0700

    Link to CAndC++.md from various places.

[33mcommit 2d6c5e8ea5df26d82e75ee543466199a0309c134[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 12:47:32 2015 -0700

    Add a note a about language support.

[33mcommit a91c717ad134c13b997afba7f9cc4c041a25dbc8[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Fri Jun 12 12:11:53 2015 -0700

    be more optimistic about APIs for C/C++ people

[33mcommit 9fe8d0f4c0943787470b6db13f98f7b9344449cc[m
Merge: 8b7b637 b807a06
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 10:40:04 2015 -0700

    Merge pull request #157 from WebAssembly/c-and-c++
    
    Start a document addressing C/C++ developers.

[33mcommit b807a062ac0fca0baae06bdd165d19637a42e2d4[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 10:30:53 2015 -0700

    Use a colon.

[33mcommit f0d1c3b9be50cc61ec9b81976ea969847b93f09a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 10:28:53 2015 -0700

    Delete title period, and reword implementation-defined behavior paragraph.

[33mcommit 70b998a63ae4aca71daac7c361d0724e07f5e009[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 10:26:36 2015 -0700

    Explain what ILP32 and LP64 mean.

[33mcommit 1b85483e01ae7a5714d3586d54e456e977e4ed63[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 12 10:04:45 2015 -0700

    Start a document addressing C/C++ developers.

[33mcommit 8b7b637cca08d5a8521b94ef4a02d2648512754b[m
Merge: bba8bdb 4f57a91
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 19:01:09 2015 +0200

    Merge pull request #155 from WebAssembly/engrish
    
    Fix sentence structure, and capitalize

[33mcommit 4f57a91a923aa1b10d47e713574712006df978f1[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 18:39:06 2015 +0200

    Moar fixes

[33mcommit 5ef82922cf0522560e0aa77452c2ec302d4f63b4[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 18:38:16 2015 +0200

    Fix capitalization correctly, per APA

[33mcommit 722498f850cca1cfd6e2eef76bf9910acd01294a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 18:23:00 2015 +0200

    Standards body.

[33mcommit bba8bdb191cfce8b8b7fb93621b5df0353034f2c[m
Merge: 4cb469b e7d0d96
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Jun 12 11:15:41 2015 -0500

    Merge pull request #152 from WebAssembly/update-portable-api-para
    
    Update API portability paragraph

[33mcommit e7d0d9625742c32888710c4a724fe5651e2826a3[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Jun 12 11:15:29 2015 -0500

    Rename Execution section

[33mcommit fcb1bb12f384abb2480af925ed43fa2f0b00a71b[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 18:13:37 2015 +0200

    Add Contributing.md

[33mcommit 233235a152f6101b92665626117ac9646c42df71[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 18:10:28 2015 +0200

    Fix sentence structure, and capitalize
    
    "Specify and implement incrementally" didn't quite make sense with the bullet points that followed. Rephrase.

[33mcommit 4cb469bdd1eaaaf0ee6c54b69e10da48823fccc1[m
Merge: 5cac0f6 e47c672
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 17:46:20 2015 +0200

    Merge pull request #153 from WebAssembly/postmvp
    
    Short document name bikeshed.

[33mcommit 5cac0f67e2704c84e55e76c76ba96c3e09173892[m
Merge: d746c31 bd13985
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 17:46:00 2015 +0200

    Merge pull request #149 from WebAssembly/generate
    
    Markdown documentation generator and checker.

[33mcommit ed9d5f66f35a9f95d6dde672fdc714e6dda1b984[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Jun 12 09:41:02 2015 -0500

    Split doc into sections

[33mcommit e8f1b6952ca1e3fda3fc3f2f9882b3da19b0bee8[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri Jun 12 09:38:03 2015 -0500

    Relink 'Web Platform' to Wikipedia; w3c/whatwg was too narrow anyhow

[33mcommit e47c672a80a2e159e1b01779f33062894e62d85b[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 14:39:36 2015 +0200

    Short document name bikeshed.

[33mcommit d746c31b66549aab29cf0e28fc8decdc1572ea3f[m
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 13:52:45 2015 +0200

    Link to MVP from TextFormat

[33mcommit bd13985dc26a09ac783bb5ad9a6e4db5cce8dcd7[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 13:33:35 2015 +0200

    Use CamelCase for class name.

[33mcommit 3cf5262efff58ed684f2b5f3aa988493ad23d66d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 13:17:14 2015 +0200

    Use argparse. Fix for files not in cwd.

[33mcommit c6e32e6e4d8f9f3242aec0ca4e99de1017a90574[m
Merge: 7570edd 8eaa519
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 11:59:03 2015 +0200

    Merge branch 'master' into generate

[33mcommit 8eaa519446b7312a0b35ac32d80d344c49b5bd18[m
Merge: 582c28f 6b703e4
Author: JF Bastien <github@jfbastien.com>
Date:   Fri Jun 12 11:53:14 2015 +0200

    Merge pull request #151 from WebAssembly/faq-usecases
    
    Faq: mention use cases

[33mcommit 6b703e430cb847b34b94961fd5913a7e8529c283[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 11:52:15 2015 +0200

    What are WebAssembly's use cases?

[33mcommit 4ee14d75ea6d788b71ab7dbdee829bf97322078c[m
Merge: 0305acc 582c28f
Author: JF Bastien <jfb@chromium.org>
Date:   Fri Jun 12 11:51:20 2015 +0200

    Merge branch 'master' into faq-usecases

[33mcommit 582c28f34419662594d1ff40f0d72091c097393d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Jun 11 16:52:23 2015 -0700

    Fix broken link.

[33mcommit b584d7177fddaae957eadf54b2ce0359d32022cc[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 18:30:09 2015 -0500

    Update API portability paragraph

[33mcommit 985b8506bbe2588905afac7e87281824844acf25[m
Merge: fddb8fc 225ce8b
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 15:47:23 2015 -0500

    Merge pull request #141 from WebAssembly/rename-incompletely-specified-behavior
    
    Change 'incompletely specified behavior' phrasing to 'limited local nondeterminism'

[33mcommit 225ce8b501b41c67a306973f623873661c5bf323[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 15:12:01 2015 -0500

    order -> sequence

[33mcommit 2546267273a7a18b6cb052ff59e7a18e41b29f12[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 14:26:57 2015 -0500

    Tweak to remove seq-cst mention (irrelevant to goal of doc) and mention nondeterminism of load

[33mcommit def439103c987bb7184bd462f5355d8b7555f679[m
Merge: 40a6bf5 fddb8fc
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 14:23:27 2015 -0500

    Merge branch 'master' of https://github.com/WebAssembly/design into rename-incompletely-specified-behavior

[33mcommit fddb8fccf0d79031ca9356a3e3b4cf75becaf239[m
Merge: b43db39 2b32b3c
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 21:16:45 2015 +0200

    Merge pull request #147 from WebAssembly/refactor-essential-post-mvp-features
    
    Refactor essential post-MVP features

[33mcommit 2b32b3ca9b5b0008d3f007686f64b1944fa96986[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 21:13:37 2015 +0200

    Suboptimal; albeit slower.

[33mcommit 26dfab5dcf0aa38c7d214cd2fd1f8fb2265882f7[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 21:12:13 2015 +0200

    JS == not fast.

[33mcommit d4b10d2fb103ab377efbba770eafc183db745a64[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 21:02:18 2015 +0200

    May support, in compilers and polyfill.

[33mcommit 0305acc56ef073b95073531d0df514140b6b832b[m
Merge: 7ee73d8 b43db39
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 20:59:00 2015 +0200

    Merge branch 'master' into faq-usecases

[33mcommit 7ee73d80f22f3694f499023cd5d0d64f95cc8a13[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 20:58:45 2015 +0200

    FAQ: use cases.

[33mcommit 71f0b245cb57e7027df94707a78d656983a99020[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 20:55:44 2015 +0200

    SJ EH.

[33mcommit b43db39909a573a4a6588a3487d50853682c15a4[m
Merge: ae19967 36b5908
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 20:52:22 2015 +0200

    Merge pull request #134 from WebAssembly/usecases
    
    Use cases. WebAssembly: what is it good for?

[33mcommit e53990a8cdc6239050290c340454e0ded08c4246[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 20:25:26 2015 +0200

    Clarify STL.

[33mcommit 6ec45bfca37b09c5b499e4b057a4f2757546b8e0[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 20:24:38 2015 +0200

    Drop 'very'.

[33mcommit a5fa6585f2d0a952fca085e1557bbce93e46997b[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 20:22:25 2015 +0200

    Branching EH.

[33mcommit 7570edd51523b2189c1574ed74b259e55f5c87ae[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 19:46:33 2015 +0200

    Markdown documentation generator and checker.
    
    Generate HTML documentation from Markdown files in the current working
    directory, and check that their links are valid.

[33mcommit 40a6bf5e08479803b9e595bd2ac9f666af09800a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 10:53:32 2015 -0500

    Refine description of thread nondeterminism

[33mcommit b1b824b45111d4f5f4aa7e95d2d7b128c4ad8431[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 10:43:10 2015 -0500

    Add period.

[33mcommit c6b5be06dd2f505e5a669e0c5bf3fb7f25c98677[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Jun 11 10:37:44 2015 -0500

    Update for comments

[33mcommit 36b59085649bf01b071064c977fc1a552f9149de[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 17:27:11 2015 +0200

    UNIX -> POSIX.

[33mcommit abe174981643df3b0f37d625425e5706e6302708[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 17:26:19 2015 +0200

    Missing 'and.'

[33mcommit 34318807a17cb43d79e8cf5939319e5b4909dc6d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 17:25:20 2015 +0200

    Toolkits.

[33mcommit 9d4c71886ff6946be202e17873329aca1e62e6b5[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 17:24:15 2015 +0200

    Drop enable.

[33mcommit 4918581daa7ca915e4d8183ea3cdd8357c13adfd[m
Merge: 9d8df3f ae19967
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 17:23:01 2015 +0200

    Merge branch 'master' into usecases

[33mcommit 83bbf1108534eb8ea8e6fa66122d5cad616b9c38[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 17:20:31 2015 +0200

    Be more formal.

[33mcommit d6845563e73f6e6eccf3398d5fc38a56ad4dbf67[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 17:17:09 2015 +0200

    s/initial release of WebAssembly/WebAssembly MVP/

[33mcommit b7d0b716fb29d8694303f96e9a48a64c51eb14cc[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 11:25:55 2015 +0200

    Refactor essential post-MVP features
    
    * Format similar to other documents.
    * Clarify threads.
    * Move some details of threads on the Web to Web.md.
    * Move some details of SIMD for the Web to Web.md.
    * Expand on zero-cost EH.
    * Move discussion of coroutines to FutureFeatures.md.

[33mcommit ae19967119bbb5f69436d5f4fa304321eb979657[m
Merge: f7e9add bebba12
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 10:23:33 2015 +0200

    Merge pull request #145 from WebAssembly/portability-more
    
    Mention two's complement integers and IEEE-754 floats.

[33mcommit f7e9addf5d424e6d9a4355878d09d9a8e0a320c4[m
Merge: eca7468 c0d9bdf
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 10:21:12 2015 +0200

    Merge pull request #144 from WebAssembly/ieee754-updates
    
    Make NaN bit patterns fully non-deterministic.

[33mcommit c0d9bdf63df30b622f35bca52ed4ae02123cc1a7[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 10:20:33 2015 +0200

    Use `code quotes`

[33mcommit eca74686db71c76ea9f9cb6f4bb6d1b896388759[m
Merge: fa8f5f3 2decb7c
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 10:02:56 2015 +0200

    Merge pull request #142 from WebAssembly/web-nonweb
    
    Wordsmith Web.md and NonWeb.md.

[33mcommit 2decb7c5e952d07f1b60a658e7972df09a821078[m
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 10:02:22 2015 +0200

    Hyphen for "high-level", as in other documents.

[33mcommit 9d8df3fd18bc68db8aad6f4bf3e24ebc772a01f9[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 09:43:42 2015 +0200

    Clarify code reuse.

[33mcommit b21682d6b49c4866010ccb5d3b01bbec8aef6425[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 09:36:57 2015 +0200

    C/C++ first. Drop Dart. Ellipsis.

[33mcommit 8182a0a528edec476101417c45063dd64fcee4b0[m
Merge: 15b3a94 fa8f5f3
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 11 09:34:56 2015 +0200

    Merge branch 'master' into usecases

[33mcommit fa8f5f3aa8dcbb5cdbd1cfcab74fd9b0f0f758a0[m
Merge: 95b6074 4e44dc2
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 11 09:33:35 2015 +0200

    Merge pull request #143 from WebAssembly/srem-notrap
    
    Make INT32_MIN % -1 defined again.

[33mcommit bebba12d8955022864f9613487973d86686f07dc[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 21:33:03 2015 -0700

    Mention two's complement integers and IEEE-754 floats.

[33mcommit 4e44dc2d06355a61ec2b259e90145dee8cfca549[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 18:16:47 2015 -0700

    Make INT32_MIN % -1 defined again.

[33mcommit 02621edd5d7b5966fc4021f2226b1b2f382f3b65[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 16:37:45 2015 -0700

    Make NaN bit patterns fully non-deterministic.
    
    It turns out that requiring NaN bit patterns to be deterministic with
    respect to opcode and operands within a given run of an application is
    too strict, because live application processes may be migrated between
    implementations.
    
    This patch also elaborates on the exceptions to IEEE-754 conformance.

[33mcommit 5a7008cb8b145d0944edcb196f663ab51e888d92[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 17:09:54 2015 -0500

    Well, not *fully* deterministic

[33mcommit e5496a88eb64a64f8dceffa0e221fdc63c82c766[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 17:05:18 2015 -0500

    Remove spaces

[33mcommit 8baea632bc43b7feb610d39621fa516e8ffee3e3[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 15:04:34 2015 -0700

    Wordsmith Web.md and NonWeb.md.

[33mcommit 95d112a39d392596cecfe841f05ab8ed96554058[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 16:59:11 2015 -0500

    Change 'incompletely specified behavior' phrasing to 'limited local nondeterminism'

[33mcommit 95b607450bdeeac671e7f0d4eba7cbfacedd21cb[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 15:48:59 2015 -0500

    Update URLs to account for spec->design repo renaming

[33mcommit 9f7ca362035d0b35a509b64a822fc6bdb7545a78[m
Merge: 7ce0884 fe888e1
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 15:13:14 2015 -0500

    Merge pull request #140 from WebAssembly/rename-polyfill-repo
    
    Update URL of polyfill repo to polyfill-prototype-1

[33mcommit fe888e1ae7a0647a692b6dc3f6b0be5b06dc8d06[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 15:09:05 2015 -0500

    Update URL of polyfill repo to polyfill-prototype-1

[33mcommit 7ce0884567759dfb7ba3638c4cda8a6249cb1ebf[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 12:33:59 2015 -0700

    Consistently capitalize "Web" in "the Web".

[33mcommit 2e062b3a294dfade8bb64fc8fe1aacd823dab586[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 12:31:54 2015 -0700

    Linkify 'mobile' and 'IoT'.

[33mcommit 4664f36242b5f9ac43bc436139c1165f830f16be[m
Merge: 615fcd5 858d492
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 10 20:57:08 2015 +0200

    Merge pull request #131 from WebAssembly/non-llvm
    
    FAQ: non-LLVM compilers.

[33mcommit 615fcd50efb22872ead6c8eaed600e2cdc30e65e[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Wed Jun 10 11:28:56 2015 -0700

    reword to clarify that ABI will NOT be standardized for MVP

[33mcommit 1728e1303c7fb896339a202a12d83597e318caae[m
Merge: 28ff0b9 33991c2
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Wed Jun 10 11:10:44 2015 -0700

    Merge branch 'abi'

[33mcommit 33991c2b678f17ee31b7d960ae12287fc61c2192[m
Merge: 0239227 28ff0b9
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Wed Jun 10 11:09:02 2015 -0700

    Merge branch 'master' into abi
    
    Conflicts:
    	FutureFeatures.md
    	MVP.md

[33mcommit 28ff0b98a0dbeae639c07e52d4750c575368e1e1[m
Merge: 4595528 7232fbd
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 10:37:36 2015 -0700

    Merge pull request #136 from WebAssembly/srem-overflow
    
    Make INT32_MIN%-1 trap.

[33mcommit 45955280c764d9de76ee6a21b06acd811bf5fc99[m
Merge: 408ceda 2c74048
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 10:35:59 2015 -0700

    Merge pull request #137 from WebAssembly/portable-addressing
    
    Wordsmith the portability of heap addressing a little

[33mcommit 408cedad56dd55343c13f0f8f3551a901db493ca[m
Merge: 7abd8a4 662f66c
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 10:24:15 2015 -0700

    Merge pull request #138 from WebAssembly/just-iot
    
    Remove the ™ from IoT.

[33mcommit 662f66ce5d78c0fb3218c532751576eb6694a78d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 10:13:04 2015 -0700

    Remove the ™ from IoT.
    
    This is meant to refer to the generic acronym
    [IoT](http://en.wikipedia.org/wiki/Internet_of_Things) rather than
    to make reference to any trademark.

[33mcommit 7232fbd9539557a31649aeee711836ae5b430687[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 09:59:56 2015 -0700

    Add code quotes.

[33mcommit 7abd8a4183674a48c25f154900a069812488ae6e[m
Merge: 6525f08 b0db228
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 10 18:58:44 2015 +0200

    Merge pull request #130 from WebAssembly/mobile
    
    Mention mobile platforms

[33mcommit 90ce54fd91e14864b4aafcf0c2f2ecf2b28ceea2[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 18:48:36 2015 +0200

    32/64 int trunc, sext, zext.

[33mcommit 2c740481c4862fc58b9e99e16652c2e1dccbf786[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 10 09:48:07 2015 -0700

    Clarify that implementations don't actually need to make 4 GiB of memory available.
    
    And that >4GiB heaps will be a feature-tested feature.

[33mcommit aaa50db3bd0d1f783f6f2db5da26e3e8fbae12f3[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 5 14:33:09 2015 -0700

    Make INT32_MIN%-1 trap.
    
    It doesn't actually overflow, but the corresponding division overflows,
    so even though we could define it to be correct, it isn't especially
    useful. Also, this operation is UB in C and LLVM.

[33mcommit 858d4923a57e5c6a90371c7a1561a043912a325d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 18:06:31 2015 +0200

    Be welcoming.

[33mcommit b0db2286067eaebe908ff0a631d119bb1d387b5f[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 17:29:59 2015 +0200

    Less buzzword, more compliance.

[33mcommit 4d4f3ba30b7158331fa5927956cd1de020be144c[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 17:21:08 2015 +0200

    Buzzword Compliant (TM).

[33mcommit 24220f541e870a2a0a9b33b09a76264f472f050c[m
Merge: 7d31cb9 6525f08
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 17:19:42 2015 +0200

    Merge branch 'master' into mobile

[33mcommit 15b3a944bbb85119d62feac5482bca1f0779757d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 17:18:28 2015 +0200

    Move NPAPI.

[33mcommit 448b9b97a58351d4c08088f7b645879fd66077d2[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 17:17:21 2015 +0200

    Disambiguate 'it'.

[33mcommit b42365f016360359290cf32d6d790470aa76de8d[m
Merge: 9ccf0fa 6525f08
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 17:16:24 2015 +0200

    Merge branch 'master' into non-llvm

[33mcommit d545dcfe9cba788e189569721a746e3047e57def[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 17:14:03 2015 +0200

    Move 64-bit integer arithmetic to MVP.
    
    The discussion in #81 leads me to believe that we mostly agree: WebAssembly should support 64-bit integer arithmetic in MVP, but not 64-bit heaps.

[33mcommit 6525f08e1c9a3a7905b60d83616c3e0e80dabe30[m
Merge: ec26f61 58eb544
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 09:56:42 2015 -0500

    Merge pull request #133 from WebAssembly/readme-moar
    
    Readme moar

[33mcommit 1b9845738c87e9ff74126c5396326520ef5c492d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 16:53:30 2015 +0200

    NPAPI: only for web APIs.

[33mcommit 8efa809874ec4edb7b47683f73931578056f38f1[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 16:52:02 2015 +0200

    Intrp & VM.

[33mcommit 3b4469295313c0c42832ef25dd204f82c992baeb[m
Merge: 2c447e3 ec26f61
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 16:51:17 2015 +0200

    Merge branch 'master' into usecases

[33mcommit ec26f61d2b51cc578f1b60199af8c4d5d5755437[m
Merge: 5d22196 363265f
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 10 16:44:15 2015 +0200

    Merge pull request #132 from WebAssembly/refactor-polyfill
    
    Refactor polyfill, discuss evolution

[33mcommit 5d22196cace4a4f89b2ea3f0232b54bd5756c08d[m
Merge: dd54e39 b419cff
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 10 16:41:17 2015 +0200

    Merge pull request #129 from WebAssembly/tooling-sanitizers
    
    Sanitizers: mention trapping and shadow stack.

[33mcommit dd54e39cce304cf1d402066138f77c860947ab45[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 09:09:13 2015 -0500

    Fix typo in Web.md

[33mcommit 9ccf0fa4b114623dee00b510de78a458c60f46e3[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 15:52:06 2015 +0200

    clang

[33mcommit 76a844643e8075b933e283d84fb9ab1a30adab15[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 15:35:14 2015 +0200

    Rephrase the question.

[33mcommit 58eb5445c450eec10b4b16cdc79b7be52b9f2ba2[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 14:35:35 2015 +0200

    Parenthesize MVP consistently.

[33mcommit 7d31cb9729c2d8dbf4214329620f60661d2a3593[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 14:33:30 2015 +0200

    Internet *all* the Things

[33mcommit 2c447e38e70a4d3ad21792f42acab309cd36a0b1[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 13:48:02 2015 +0200

    Use cases. WebAssembly: what is it good for?
    
    Unordered and incomplete list of use cases for WebAssembly.

[33mcommit 60045735c8912e2061fb0bf211e20fe3db72dd91[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 13:30:16 2015 +0200

    Typo.

[33mcommit 37092b32f4034d31fbce4da07137052b385ae256[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 13:29:12 2015 +0200

    Tune README
    
    * Change the title to 'design' instead of 'specification' as discussed in #80. I'll change the actual repo name later.
    * Mention that the spec will be elsewhere.
    * Clarify that this is tentative.
    * Link to the public mailing list, which I expect people will want to use once the repo becomes public (discussion directly on github would be painful).

[33mcommit 363265f4bc89b8eeb16243483f53c4217ea573c4[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 12:06:47 2015 +0200

    Refactor polyfill, discuss evolution
    
    * Refactor to give this document the same feel as others.
    * Mention PNaCl alongside Emscripten and asm.js.
    * Use 'a polyfill' or 'an asm.js polyfill' instead of 'the polyfill', as there can be many polyfills.
    * Discuss polyfill evolution, summarizing discussion from #96.

[33mcommit 80453e9170260252565f16b118455d1fc5f17423[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 11:30:48 2015 +0200

    FAQ: non-LLVM compilers.

[33mcommit c986e51e80093781f048ad9adfc2dcbdff8f57bf[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 11:21:40 2015 +0200

    Mention mobile platforms
    
    Mobile guides some of WebAssembly's concerns, but wasn't mentioned anywhere so far. This addresses #97.

[33mcommit b419cff441426d04b3f1762c18001cc57ec4f973[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 10:55:19 2015 +0200

    Sanitizers: mention trapping and shadow stack.
    
    As discussed in #125:
      https://github.com/WebAssembly/spec/pull/125#discussion_r32023765

[33mcommit c2fcc21766d77bf6ca7758a5e04833ed81d10bdc[m
Merge: 29e8644 6c1128a
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 10 10:43:51 2015 +0200

    Merge pull request #128 from WebAssembly/web-security
    
    Web security: CORS and SRI

[33mcommit 6c1128a925f8fb3a6e44d0effe30d36984dd0d54[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 10:42:17 2015 +0200

    Web security: CORS and SRI
    
    As discussed in #125:
      https://github.com/WebAssembly/spec/pull/125#discussion_r32024524

[33mcommit 29e86447dab47e20252c12d684b877b15a74bd71[m
Merge: 2b1258a 6bd512e
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 10 09:02:25 2015 +0200

    Merge pull request #123 from WebAssembly/portable
    
    Define portability

[33mcommit 6bd512eaa9982bb62b06be2d0bc6bc6636d3253e[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed Jun 10 09:00:41 2015 +0200

    Remove tier 1.

[33mcommit 2b1258a28e074ce4ad5db7d00d62b1ea6b152d4d[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Jun 10 00:06:26 2015 -0500

    Add Epic demo to table

[33mcommit 5784ed90a07be7ad72521644782d0707922f9cf5[m
Merge: e1a343c 5d72236
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue Jun 9 17:56:40 2015 -0500

    Merge pull request #118 from WebAssembly/FAQ-tweaks
    
    Some FAQ updates based on discussions after the FAQ merge

[33mcommit d1b0bf71f8f1942e2093f21578097dc9cba34266[m
Merge: d0e928a e1a343c
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 23:27:25 2015 +0200

    Merge.

[33mcommit e1a343c9a7a7f92bdda1b6306aabcfa7eddb1f30[m
Merge: 4e47a65 3c6c2f6
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 9 23:23:58 2015 +0200

    Merge pull request #125 from WebAssembly/refactor-future-features
    
    Refactor future features

[33mcommit 3c6c2f67f3438d0c324b357711db4f4804c79566[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 23:17:22 2015 +0200

    Drop MAP_FIXED, add to Tooling.md in a separate PR.

[33mcommit d0e928ac6d57f75a139f99f6cbd73024661e76e3[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 23:02:14 2015 +0200

    Change tense.

[33mcommit e4e81281e14c6b37853ab838120ba6242bd51e94[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 22:57:49 2015 +0200

    Unaligned can trap.

[33mcommit 02392279b0c5d5f37bea0c9989ddda1c88080205[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Tue Jun 9 11:01:28 2015 -0700

    refine comments on ABI

[33mcommit fc88967d7bb150a3a82101d631025fa6da7d24ee[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 18:16:04 2015 +0200

    Clarify 64-bit heap.

[33mcommit 82bd80a0613f269bf3293013c0b953eb8f69140d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 18:12:40 2015 +0200

    Clarify shared memory.

[33mcommit 7f7713be0e489fdd8f829b8fe841ef93f9c3beba[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 17:17:59 2015 +0200

    Drop CORS and SRI. Move to MVP in another PR.

[33mcommit d69b6dba446cbfe19438ab16b6813d525088945a[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 17:15:27 2015 +0200

    Drop ABI mention, it's hard to explain concisely.

[33mcommit 4188c7b9d823d0d19bc355607ff1f702f7760c33[m
Merge: ad83b19 4e47a65
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 17:14:22 2015 +0200

    Merge branch 'master' into refactor-future-features

[33mcommit 4d1bd0d23cfd63ec7fc459bb276c969785d956a7[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 16:58:51 2015 +0200

    Move 'as-if' up. Clarify. Explain going forward. Reword NX.

[33mcommit 4e47a65bea188578ee41484b02a6191e79e7f0e7[m
Merge: d068cf2 cb9e2b5
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 9 16:40:15 2015 +0200

    Merge pull request #115 from WebAssembly/refactor-mvp
    
    Refactor MVP

[33mcommit cb9e2b52f422bce27e689d7e9f025a58d2a8bc18[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 16:27:43 2015 +0200

    Link to dynamic linking from Web.md.

[33mcommit b765304ee688c01f3656185c2ac4750081c2327e[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 16:25:16 2015 +0200

    Clarify first paragraph of Web.md.

[33mcommit 48b1f14257cb2ddeaeb7d3390425991c2c087e4b[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 16:16:28 2015 +0200

    Stray spaces.

[33mcommit 54671fb0bd6689789cefcd26215d0bb7a42bb40c[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 16:11:34 2015 +0200

    Missing space.

[33mcommit bfc67e8b9671bfd7b2be1f646ba40481f125538e[m
Merge: bbdb216 d068cf2
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 16:10:57 2015 +0200

    Merge branch 'master' into refactor-mvp

[33mcommit 4c797055b4d42b5a0b7b68d0b08ca32c8ab092bf[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 16:04:44 2015 +0200

    Clarify tier 1.

[33mcommit 3f83acabd167609e0982f5fbbdb817ab2cf759d8[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 15:38:23 2015 +0200

    Clarify abstraction layer.

[33mcommit eaf86e8baabbfa016a68459702f18b79822412e4[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 14:32:49 2015 +0200

    Unaligned.

[33mcommit da1c71442ca57ba55297b464b22c08843e05429e[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 14:31:42 2015 +0200

    Byte-addressable.

[33mcommit 77a3606db1b67410d30c6a2414da7aff0357d0eb[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 14:30:41 2015 +0200

    Clarify 'as-if'.

[33mcommit 5f66b4acf05d297914af62a7fa58423c7465ddec[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 14:21:26 2015 +0200

    Drop modern browser features.

[33mcommit 9b60ad85a99332076713a5704c823f7522ad677b[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 14:17:00 2015 +0200

    Remove ILP32, and link to future features for 64-bit heap.

[33mcommit ad83b196041dcbffda35ab9ec9afc1504b9ae48c[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 13:33:49 2015 +0200

    Finish refactoring. Address #49.

[33mcommit bf2555a63e3e59dd5c021a78d9d4e4d1874e2a74[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 13:12:27 2015 +0200

    Refactor future features
    
    This refactoring clarifies points made in #53, #99, #81, and overall tries to make the text more self-coherent, less bullet-pointy.

[33mcommit 258e41a0e019c79267943ffb4bc83bcae2f8ea1f[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 12:46:02 2015 +0200

    Reword around ILP32.

[33mcommit d068cf28619ab4cb5902798e9ee281691b8d8f43[m
Merge: 6db3151 6b70297
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 9 11:33:35 2015 +0200

    Merge pull request #124 from WebAssembly/gitignore
    
    Add .gitignore

[33mcommit 6b702979858d9e8d454d3660ab4b1160021d1cf2[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 11:32:49 2015 +0200

    Add .gitignore

[33mcommit 01676f85cbb672aa1199e35e2a83a480aacff20e[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 11:27:11 2015 +0200

    Define portability
    
    Address issue #38.

[33mcommit bbdb2160721bb8b58706a73e12f5d05f500a2052[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 09:19:56 2015 +0200

    Fork out Web and NonWeb documents.

[33mcommit d4216022b0f368923eab6a43e1022e8c25d6efc8[m
Merge: ca62f96 6db3151
Author: JF Bastien <jfb@chromium.org>
Date:   Tue Jun 9 08:39:55 2015 +0200

    Merge branch 'master' into refactor-mvp-2

[33mcommit 5d722360b6680cac7d6d8b31157ed76ca994180a[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Mon Jun 8 11:15:01 2015 -0700

    Some FAQ updates based on discussions after the FAQ merge

[33mcommit 7792d21fe170e955938369009776979b6232128a[m
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Mon Jun 8 13:25:45 2015 -0700

    Add small note on ABI
    
    Adding to mention current state of discussion of ABI per #67

[33mcommit 6db31519e7e64cf8e5559962e176c1c0a05cd765[m
Merge: b37f0aa e5cc7b9
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 8 13:20:03 2015 -0700

    Merge pull request #102 from WebAssembly/incompletely-specified-behavior.md
    
    Incompletely specified behavior.md

[33mcommit ca62f96dcbf265a2cda6a0b794e2107e7f86f097[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 20:04:59 2015 +0200

    Clarify _start/init examples.

[33mcommit 39aaca82754ba55759790f333ad57036bc2ca620[m
Merge: 5748016 b37f0aa
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 19:56:53 2015 +0200

    Merge branch 'master' into refactor-mvp

[33mcommit b37f0aa8ee2344b600d21d563c426226fb66dacc[m
Merge: f072fd1 3380236
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 8 19:55:25 2015 +0200

    Merge pull request #114 from WebAssembly/polyfill-moar
    
    Some work on Polyfill.md

[33mcommit 5748016d43761c4f86491b9e2612e33956b03b7d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 19:49:42 2015 +0200

    Remove wordy section. It's explained a few lines below.

[33mcommit 658f9498b49ea93a0c7ee9fb03533e2eeb273b96[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 19:48:10 2015 +0200

    Gramer fixx.

[33mcommit 34c726663829d4e87f0129ea3c1213aa44a132d8[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 19:46:46 2015 +0200

    Un-bullet the text format.

[33mcommit 93d806486f48bd1b792a8396d09096a65d0093a3[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 19:08:46 2015 +0200

    Final edits to MVP.

[33mcommit f072fd1407b5edc9c7300f3b0fcfc37d377b66f7[m
Merge: c57ff47 fb2890c
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 8 18:59:23 2015 +0200

    Merge pull request #117 from WebAssembly/js-and-wasm
    
    Add JS Q to FAQ

[33mcommit fb2890c6e7587774e152ac233387e79d29b37beb[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 8 10:58:39 2015 -0500

    Update FAQ.md

[33mcommit 98c600813079c441dbfd8e1b980e16b1a6dedf39[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 8 10:57:18 2015 -0500

    Add JS Q to FAQ

[33mcommit 8ab1c1eaaf25e0688d893a70708aba31188f76d1[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 17:46:01 2015 +0200

    Split out text format into its own document.

[33mcommit 9329862be20ddb6da38e28519871910c2680af1d[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 17:38:49 2015 +0200

    Binary format.

[33mcommit 0b269f854d28e189cff9057a6d2441325b678ecd[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 17:24:42 2015 +0200

    Format-fu

[33mcommit c57ff47cb2b26dd186349a87b92cf012e410a904[m
Merge: 75daacc 7d98f7e
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 8 08:24:25 2015 -0700

    Merge pull request #116 from WebAssembly/start-faq
    
    Create FAQ.md

[33mcommit 7d98f7ec1292ad4ef4b7871d6c4aafdf3934d916[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 8 10:12:21 2015 -0500

    Update README.md

[33mcommit 8e67a7e47c6a4c4ab4940e42501b6c8e6ae7eb38[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 8 10:09:40 2015 -0500

    Create FAQ.md

[33mcommit 4349490e7895e77e286541dc637b263670131e52[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon Jun 8 17:01:52 2015 +0200

    Refactor MVP
    
    Much of MVP's content is polyfill implementation detail, or design rationale. Move things around so that MVP links to the relevant documents instead. This reduces some repetition in the documents, and makes it easier to dive into the high-level of MVP without having all the details.

[33mcommit 75daacc702230a1cc3f5dd17f92aaeb7b00ec701[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Fri Jun 5 14:30:26 2015 -0700

    Change "MVP" to "the MVP" to fit the context.

[33mcommit 33802369de1fec0c017c3b351f4a445ed0921387[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Thu Jun 4 15:56:07 2015 -0700

    some work on Polyfill.md

[33mcommit 8eff2a4768fd5f8a1506c2b3d48294d7abb73004[m
Merge: a6272b1 0b450fe
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 4 17:36:36 2015 -0700

    Merge pull request #113 from WebAssembly/feature-test
    
    Add a new feature test section, link to it.

[33mcommit 0b450fe855ab25cde82ba10c78e2139e75df8b94[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 4 17:36:20 2015 -0700

    Address sunfish's comments.

[33mcommit bf2184fd6daf4e51109c27ccaff8b826d63bd184[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 4 15:50:23 2015 -0700

    Add a new feature test section, link to it.
    
    This is an initial skeleton. It's important to help people understand what we mean by feature test, and we've discussed it enough that I think we have a basic agreement of *what* we want to do, just not design of exactly *how* to do it.

[33mcommit a6272b1c5d7219bd1ba8e6452ac0b905a2e95acf[m
Merge: 5510bdf d94d55d
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Jun 4 13:34:50 2015 -0700

    Merge pull request #111 from WebAssembly/polyfill-js
    
    Polyfill to JavaScript

[33mcommit d94d55d8a900fa507ecfc59d37bba0ff33b70271[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 4 13:22:59 2015 -0700

    Effective and efficient.

[33mcommit e5ae3cf844ded15daada4dbaf4afff90ffee4c05[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 4 13:09:12 2015 -0700

    Refactor polyfill quite a bit.

[33mcommit cd35f9b39f4d8bc186ea8b8fb889dc451113b8d7[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 4 12:46:48 2015 -0700

    Drop 'at high speeds', polyfill is already 'effective'.

[33mcommit 100adbb532958c2c0fd35979b22cf2b8155908be[m
Merge: adeeb06 5510bdf
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Jun 4 11:00:13 2015 -0700

    Merge, and drop the MVP change to asm.js. Discuss separately.

[33mcommit e5cc7b9a30a81e18ca8706daa09ca6912af027b9[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 2 07:11:09 2015 -0700

    Say "floating point" instead of "floating-point".

[33mcommit 766d32362828ba263977faca493ef82a132b709e[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 2 02:10:26 2015 -0700

    Add a section discussing C and C++.

[33mcommit eac29775795b034a65724b1de71d2db31f91a1cd[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 2 02:09:41 2015 -0700

    Add a paragraph describing about control flow properties.

[33mcommit de455af1cd7e94e5940262f1511b1be5fc8dc1a3[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 2 02:08:59 2015 -0700

    Tighten up the language about observable differences between implementations.

[33mcommit 5510bdf079057f42ff35511108c4db50196d540d[m
Merge: cbd6b70 3cd3dde
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 3 15:35:42 2015 -0500

    Merge pull request #109 from WebAssembly/mvp
    
    Rename "V1" aka "v.1" to "MVP".

[33mcommit adeeb06bed5a14ea655b6ed2830824734f9f29af[m
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 3 13:25:25 2015 -0700

    Polyfill to JavaScript
    
    As [discussed](https://github.com/WebAssembly/spec/issues/96#issuecomment-108582323) asm.js is a smart implementation strategy to get polyfills to execute fast, but isn't a requirement. Some features aren't supported by asm.js, and a polyfill may decide to avoid asm.js on some browsers when performance isn't quite there.

[33mcommit 3cd3dde7eb1b90abfcfa9b100ca6afd09237a12e[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Wed Jun 3 12:16:54 2015 -0700

    Rename "V1" aka "v.1" to "MVP".

[33mcommit cbd6b7024887ce0ba7b84f2f99e4df3568bf43b6[m
Merge: aa59f86 dc8c7a7
Author: JF Bastien <github@jfbastien.com>
Date:   Wed Jun 3 09:14:00 2015 -0700

    Merge pull request #101 from WebAssembly/tooling
    
    Tooling: process dump, and all-in-WebAssembly

[33mcommit dc8c7a73a5799137d200bd3fd3a76a532a8e6268[m
Author: JF Bastien <github@jfbastien.com>
Date:   Tue Jun 2 08:49:47 2015 -0700

    Update Tooling.md

[33mcommit aa59f86f3e2b49976fd429c8b74d9b97e62aa072[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue Jun 2 08:48:09 2015 -0700

    Fix spelling errors.

[33mcommit 4dbe3afc54b5f99eeefa19cb5070a67e45bf9396[m
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 1 15:35:58 2015 -0700

    Tooling: process dump, and all-in-WebAssembly

[33mcommit d84e12434982efa2ac5b1d97efe038d305cf8dab[m
Merge: e14b446 cd8aae9
Author: JF Bastien <github@jfbastien.com>
Date:   Mon Jun 1 12:54:56 2015 -0700

    Merge pull request #92 from WebAssembly/incompletely-specified-behavior.md
    
    Clarify the incompletely specified behavior wording

[33mcommit e14b446e0be23cdd7e264659777db9693d6c3b2c[m
Merge: 73925dd 0986668
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon Jun 1 00:45:43 2015 -0500

    Merge pull request #71 from WebAssembly/update-modules-section
    
    Improve code loading and modules section

[33mcommit cd8aae95e274a823602c10e3bcba18ef7b58db0a[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon Jun 1 20:13:03 2015 -0800

    Say "applications" instead of "programs".

[33mcommit 9d9381745b8fe6b797b241d6298e73165cd88e1b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 28 11:06:57 2015 -0700

    Give a brief explanation of why nasal demons are undesirable.

[33mcommit 73925dd48b27327f1175501446c57de0080be257[m
Merge: 357f620 8d946bf
Author: JF Bastien <github@jfbastien.com>
Date:   Thu May 28 10:41:04 2015 -0700

    Merge pull request #87 from WebAssembly/incompletely-specified-behavior.md
    
    Create a list of incompletely specified behavior.

[33mcommit 8d946bf1e216531637f53904ea51381baafbae9f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 28 09:39:41 2015 -0700

    Create a list of incompletely specified behavior.

[33mcommit 357f62025d08d40a2172972ebd0f4504020c66c2[m
Merge: 1a6356c 321f252
Author: JF Bastien <github@jfbastien.com>
Date:   Wed May 27 17:11:32 2015 -0700

    Merge pull request #86 from WebAssembly/long-simd
    
    Link to long SIMD proposal.

[33mcommit 321f252c323708782b5e829eedac9c36aedf0587[m
Author: JF Bastien <jfb@chromium.org>
Date:   Wed May 27 13:43:58 2015 -0700

    Link to long SIMD proposal.
    
    We still need to discuss long SIMD in details for Web Assembly, but adding a link to the SIMD.js bug tracker won't hurt.

[33mcommit 1a6356c237a04734496956bf27bb41360c51050e[m
Merge: a462708 f948b32
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue May 26 13:30:10 2015 -0700

    Merge pull request #60 from WebAssembly/misc-floating-point
    
    Further work on floating point semantics

[33mcommit f948b32c20a58f26b790e160056525c5f191887b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 06:41:52 2015 -0800

    Say "floating point" instead of "floating-point".

[33mcommit 9cf67f90e2601d54ea623bbdfded9219eb37126c[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 06:41:48 2015 -0800

    Reorganize the floating point semantics description.
    
    Put the mention of IEEE-754 in one place, next to the floating point
    operations, so that we can also list the exceptions next to it. Also
    mention the rounding mode and exceptions here.

[33mcommit df7957eaf8f80d4c923ad1672dd3a41a4ab22991[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 06:41:46 2015 -0800

    Add a few more floating point operations to consider.

[33mcommit d1153bd79a098a26c2326a12d4c8af21550aefde[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 06:41:43 2015 -0800

    Remove unclear interpretation of IEEE-754.

[33mcommit c9983e3256c2b72a7c23b2c33b87af1e4d57151f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 06:41:36 2015 -0800

    Make NaN contents unspecified.

[33mcommit a4627086b0b9df778ffd77c2a912e5a1a3c41e0a[m
Merge: 072d274 a6858e9
Author: Michael Holman <michael.holman@microsoft.com>
Date:   Fri May 22 11:29:23 2015 -0700

    Merge pull request #76 from WebAssembly/polyfill-page
    
    move specifics of polyfill to new doc

[33mcommit 072d2741c9188b2ffff6a71d2158921c1df6149e[m
Merge: 0435d4a df68fdb
Author: titzer <titzer@google.com>
Date:   Fri May 22 19:50:20 2015 +0200

    Merge pull request #79 from WebAssembly/add_local_types
    
    Add section on Memory types versus Local types

[33mcommit df68fdb1d1e1e66d9eddf7a39d170cd18ddfe127[m
Author: titzer <titzer@google.com>
Date:   Fri May 22 17:48:03 2015 +0200

    Update AstSemantics.md

[33mcommit 5218add4d46e74fa7a7307ae3797c24b9af688f7[m
Author: titzer <titzer@google.com>
Date:   Fri May 22 17:46:09 2015 +0200

    Update AstSemantics.md

[33mcommit 8f33c84d509bc5f667c1f4282ef284e69434823d[m
Author: titzer <titzer@google.com>
Date:   Fri May 22 16:15:15 2015 +0200

    Update AstSemantics.md

[33mcommit 09866685a2e20d20bfb6f2203fde81ebd0e004bc[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 18 23:41:49 2015 -0500

    Improve code loading and modules section

[33mcommit a6858e9a135e6affff929aac82f5f7f7afb81c62[m
Author: Michael Holman <Michael.Holman@microsoft.com>
Date:   Wed May 20 15:02:41 2015 -0700

    fixed typo

[33mcommit 95e684c2e75d08eead40dfbd315171d96fae99e1[m
Author: Michael Holman <Michael.Holman@microsoft.com>
Date:   Wed May 20 13:36:25 2015 -0700

    noted polyfill's benefit for experimentation

[33mcommit 0435d4a03083ae396f94b8f86cff42cabadd8c97[m
Merge: 973e116 5bb9f66
Author: JF Bastien <github@jfbastien.com>
Date:   Wed May 20 09:47:24 2015 -0700

    Merge pull request #77 from WebAssembly/check-overrecursed
    
    Mention that stack overflow is checked.

[33mcommit 5bb9f66025e228a1fe960a72b8031d4b7772fda9[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue May 19 19:16:44 2015 -0700

    Mention that stack overflow is checked.

[33mcommit 75d494049641c4239e1dd675099974123ebd77bb[m
Author: Michael Holman <Michael.Holman@microsoft.com>
Date:   Tue May 19 14:58:48 2015 -0700

    added note that ensuring alignment is a good thing

[33mcommit e329944ed853107b31fb7af800388567c355751a[m
Author: Michael Holman <Michael.Holman@microsoft.com>
Date:   Tue May 19 14:02:59 2015 -0700

    resolved review comments

[33mcommit 2c74542af4efe28310006c11602cf014889fe2d6[m
Author: Michael Holman <Michael.Holman@microsoft.com>
Date:   Tue May 19 13:17:33 2015 -0700

    move specifics of polyfill to new doc

[33mcommit 973e116fdfa72cc86025f677125da2da125b55ea[m
Merge: bf02b77 8c32721
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Tue May 19 12:07:18 2015 -0700

    Merge pull request #73 from WebAssembly/text-format-std-2
    
    Clarify text format standardization.

[33mcommit 8c32721a1968156748974f9be002e89ac26f9c69[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue May 19 11:41:13 2015 -0700

    Address Luke and Alon's comments.

[33mcommit 71f87c27de8ae1a7bbc562bd031896429a3a8006[m
Author: JF Bastien <jfb@chromium.org>
Date:   Tue May 19 08:06:10 2015 -0700

    Rebase.

[33mcommit 828a6898cc34aabcb688b89e6e2861d0a23479cf[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon May 18 12:36:41 2015 -0700

    Standard text format, but only for tooling.

[33mcommit 6b379cd9b38f5b7877237121455bc95631bb76bb[m
Author: JF Bastien <jfb@chromium.org>
Date:   Mon May 18 11:30:38 2015 -0700

    Clarify text format standardization.

[33mcommit bf02b778025e97098c84bc28f081163170dfd70f[m
Merge: 8587eca bc04745
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue May 19 09:54:35 2015 -0500

    Merge pull request #72 from WebAssembly/improve-essential-post-v1
    
    Improve EssentialPostV1Features summary

[33mcommit bc04745db055bfae85c68ebf1bbe4400404961de[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 18 23:55:31 2015 -0500

    Improve EssentialPostV1Features summary

[33mcommit 8587eca9aac9ffb5388abcdd720fbb8885a6fd4c[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 18 22:25:15 2015 -0500

    Fix link

[33mcommit fec52b40618fe6ad45c03075b057e45fbe6218de[m
Merge: 846bfb2 de382b9
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 18 20:36:44 2015 -0500

    Merge pull request #69 from WebAssembly/isomorphic
    
    Clarify isomorphism between binary and text formats

[33mcommit de382b9174d488becba935109cac2e18614c1982[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Mon May 18 18:11:34 2015 -0700

    clarify isomorphism

[33mcommit 846bfb2eddb1ee58627ae87bb6c04236bfd107ed[m
Merge: 61cc7a7 88b4fca
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 18:03:26 2015 -0700

    Merge pull request #68 from WebAssembly/the-reason-for-the-binary-format
    
    Further clarify that efficiency is the reason a binary format was cho…

[33mcommit 88b4fca997d44480263aa7a5fd370bc2df4a985f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 18:00:58 2015 -0700

    Further clarify that efficiency is the reason a binary format was chosen.
    
    See https://github.com/WebAssembly/spec/pull/62#issuecomment-103284882
    and subsequent discussion.

[33mcommit 61cc7a79faac0aef8c7cceabf58134a46c882fa0[m
Author: Michael Holman <Michael.Holman@microsoft.com>
Date:   Mon May 18 16:25:36 2015 -0700

    fix couple typos

[33mcommit c9451bd724f01cc861c03cadd1efc74aa2e89f7f[m
Merge: e086687 84d4f24
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 18 18:19:57 2015 -0500

    Merge pull request #66 from WebAssembly/typed-arrays
    
    Mention typed arrays as a GC thing we will want to support

[33mcommit 84d4f243b20ae23bd367d66a8abfe428963363e8[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Mon May 18 16:02:12 2015 -0700

    mention typed arrays as a GC thing we will want to support, alongside other things from JS

[33mcommit e086687948c6073cbf5e32095a13588a50fdd037[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 15:48:30 2015 -0700

    Fix a typo.

[33mcommit e5931a566f1f401043ee06c5f2b2f844e19fc09e[m
Merge: 7089031 e82520b
Author: JF Bastien <github@jfbastien.com>
Date:   Mon May 18 15:21:02 2015 -0500

    Merge pull request #61 from WebAssembly/further-tidying-of-high-level-goals
    
    Further tidying of high level goals

[33mcommit e82520b3762f284d807cb64c9067382cc860c12c[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 10:00:22 2015 -0700

    Tidy up HighLevelGoals even more

[33mcommit 70890311b86d0863457083bf6813ec802df24bc2[m
Merge: 0e16e36 afb144e
Author: JF Bastien <github@jfbastien.com>
Date:   Mon May 18 13:21:04 2015 -0500

    Merge pull request #62 from WebAssembly/clarify-binary-format-purposes
    
    Clarify the primary purposes of the binary format.

[33mcommit 0e16e36625b6c1def1fd8c892e97472f9f9b70ae[m
Merge: de13b77 1b97136
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 18 13:04:10 2015 -0500

    Merge pull request #59 from WebAssembly/update-binary-encoding
    
    Update binary encoding following recent talks

[33mcommit afb144e70f16b594c3909fd66decf4a01be29c41[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 09:13:34 2015 -0800

    Clarify the primary purposes of the binary format.

[33mcommit 3060ccfafbdde5fda2e3125d284db2998d2b89f1[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 18 09:58:01 2015 -0700

    Capitalize "Web".

[33mcommit 1b971360f1429fe66bb13d03c5d90102991d830c[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Thu May 14 13:20:20 2015 -0700

    update binary encoding following recent talks

[33mcommit de13b77d7216f566c47568b99d4ec48e60953ba1[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 14 15:57:28 2015 -0500

    Typed Objects are not ES6

[33mcommit 7d7313da95f5bd3ba45095bf23208bf544c209bb[m
Merge: eac2689 c3e8d2c
Author: JF Bastien <github@jfbastien.com>
Date:   Thu May 14 11:37:17 2015 -0500

    Merge pull request #57 from WebAssembly/long-simd
    
    Materialize the section on "Long SIMD".

[33mcommit c3e8d2cc7370cab786a4ce6ed48ef15ec51019bf[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue May 12 15:41:58 2015 -0700

    Materialize the section on "Long SIMD".
    
    This is meant to address the original concern in
    https://github.com/WebAssembly/spec/issues/41

[33mcommit eac2689004b36f5970b14fc9049706574181268f[m
Merge: fac3a4c 1f95874
Author: JF Bastien <github@jfbastien.com>
Date:   Wed May 13 12:40:38 2015 -0500

    Merge pull request #55 from WebAssembly/clarification
    
    Clarify some behaviors that were listed as needing further clarification.

[33mcommit 1f958742d76224c50a360eb2bcf2ab258a496616[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue May 12 13:34:33 2015 -0700

    Add a FutureFeatures section about trapping and non-trapping instructions.

[33mcommit 2caa6b36313c49b1248d1984e19ec6b6ce0463c8[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue May 12 11:18:59 2015 -0700

    Introduce a new concept of trapping.
    
    Replace "throws" with "traps". In practice in v.1 trapping means
    throwing, but in the future other things may happen.

[33mcommit 33102a76c7c75756c55f50444b84ed97784382ee[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue May 12 05:46:02 2015 -0700

    Clarify some behaviors that were listed as needing further clarification.
    
    Integer division by zero and signed integer division overflow both trap:
    
     - If either of these happens, it's reasonable to call it a bug in the application.
     - Integer division by non-constant values is already expensive and
       somewhat rare. A few range checks here on hardware which doesn't trap
       is a reasonable expense for portability and sanity.
    
    Shift counts are unsigned and unmasked:
    
     - Of the plausible alternatives, this is the most intuitive behavior, given
       that shifts already have silent overflow/truncation.
     - This will have a significant cost on certain applications on "tier 1"
       platforms. However, hardware behavior here varies even within "tier 1",
       so the main alternative is to return implementation-specific values,
       which harms portability. We specifically want to discourage "x86-only" or
       "ARM-only" applications from becoming common.
     - On x86, there's even inconsistency between scalar and SIMD, so something
       somewhere has to bend to accomodate it in a reasonable way.
     - SIMD.js is currently proposed to have unsigned and unmasked shifts too.
    
    Failure in converting floating-point to integer traps:
    
     - Applications may wish this to saturate to INT32_MIN/INT32_MAX, or to
       return a designated value. Or it may be a bug. We could theoretically spec
       multiple opcodes to let applications pick the behavior they want, but it's
       not currently clear how important that is.
     - Among "tier 1" platforms there are different behaviors for such conversions,
       so anything other than implementation-specific values is already going to
       require extra code on some platforms.
     - SIMD.js is currently proposed to trap in this case too.

[33mcommit fac3a4ca2be9104a4b3780f032dcd2341ae738c7[m
Merge: 29eddca 393240e
Author: JF Bastien <github@jfbastien.com>
Date:   Mon May 11 19:06:41 2015 -0500

    Merge pull request #50 from WebAssembly/convert-semantics
    
    Clarify language about rounding and behavior that requires further cl…

[33mcommit 393240edede217da17c949715bf94571a7b6d13f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 7 12:38:58 2015 -0700

    Clarify language about rounding and behavior that requires further clarification.

[33mcommit 29eddca11254da809f05fb3900b2b3f03d1cd072[m
Merge: d543ff1 df965e3
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 11 10:05:31 2015 -0700

    Merge pull request #46 from WebAssembly/more-ops
    
    Add more operations.

[33mcommit d543ff1a157910e37be1e0d31a9a965178abfc3a[m
Merge: d63ba5f 58720d3
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 11 08:44:49 2015 -0500

    Merge pull request #52 from WebAssembly/sunfishcode-redundant-jit-section
    
    Remove redundant JIT section

[33mcommit 58720d36c226ae5dfde6819fa5a5e440625fe87b[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Sat May 9 12:25:39 2015 -0700

    Remove redundant JIT section
    
    There were two sections talking about JITing. This removes one. Its content is basically covered by the other already.

[33mcommit d63ba5f449edb797c9867337790f2bd4a36618d3[m
Merge: 04067d0 6b877dd
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 8 09:09:29 2015 -0500

    Merge pull request #51 from WebAssembly/polyfill-section
    
    Tidy up HighLevelGoals

[33mcommit 6b877dd831586144ce3854bcb5963949b01e2301[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 22:01:16 2015 -0500

    Tidy up HighLevelGoals

[33mcommit 04067d07993830410958f72e144ef517f0a28956[m
Merge: 3dae108 b653a76
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 7 13:19:10 2015 -0700

    Merge pull request #48 from WebAssembly/spelling
    
    Fix spelling errors.

[33mcommit b653a768509461fb0232bb0f5d322c1e944794fc[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 7 13:13:46 2015 -0700

    Fix spelling errors.

[33mcommit 3dae10836f0c4e1508ce7f2d57ba8c20188d8537[m
Merge: f478231 7e924a5
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 7 12:53:01 2015 -0700

    Merge pull request #32 from WebAssembly/jit-optimization
    
    Discuss optimization for JITs.

[33mcommit f478231b4fde97b29843ea2f17db604850e0f121[m
Merge: c538e60 3cb6c92
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 14:50:50 2015 -0500

    Merge pull request #34 from WebAssembly/update-heap
    
    Import more recent discussion on heap accesses

[33mcommit 3cb6c92c81d3666c47a124a18155f3ef6bdbcb88[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue May 5 17:23:46 2015 -0500

    Import more recent discussion on heap accesses

[33mcommit df965e349e80c0993f8e38244f1be959de05c916[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu May 7 08:11:10 2015 -0700

    Add more operations.
    
    * The following floating-point opcodes are important fundamentals I
      think we should include in the platform from the beginning:
    
       * Sqrt - All important hardware has sqrt, asm.js has sqrt, there are no
         precision tradeoffs to consider, and it's a commonly used operation.
    
       * Neg - IEEE-754 requires that negate be a "quiet-computational sign
         bit operation", and while we don't support alternate rounding modes or
         floating-point exceptions, we do have NaNs, so we should have a Neg
         which guarantees to just flip the sign bit.
    
       * Copysign - Along with Neg and Abs, this finishes the set of IEEE-754
         quiet-computational operations. It is also nice to have this operation
         instead of the synthisized form with reinterpret-casting to integer
         and back so that the value doesn't appear to leave the floating-point
         register file.
    
    * The following items added to "Operations under consideration". They
      aren't directly available in asm.js, so they're not essential for v1,
      but they're worth considering:
    
       * Int32Ctz - This can be synthesized as popcnt(~x & (x - 1)), but that
         references the input multiple times, which breaks the single-use
         property, so having this as a regular operation is useful.
    
       * Int32BSwap, Int32Rotr, Int32Rotl - These can be synthesized too, but
         their synthesized forms also reference the input multiple times.
         Either of rotl or rotr can be synthesized in terms of the other, so
         in theory we'd only need one of those if we wanted to keep it minimal.

[33mcommit c538e60ba9333adac06f2513e2ccf76cefa8918c[m
Merge: 78ea0ca df97153
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 13:43:30 2015 -0500

    Merge pull request #29 from WebAssembly/whats-an-intrinsic
    
    Clarify intrinsification criteria

[33mcommit df971534c3ae0a7702f28117162cac338e4c24eb[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 11:22:50 2015 -0500

    Add section on intrinsics and rationale

[33mcommit 78ea0caaa46abc6613881a1e4c0dcfbc8d9d91c8[m
Merge: bf5ab6a 2aea114
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 11:06:54 2015 -0500

    Merge pull request #31 from WebAssembly/update-function-pointers
    
    Move and merge 'Function pointers' section from V1.md into AstSematics.md

[33mcommit 2aea11463d41cc0bfe4c7b6d7c3b9b6546c36b14[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue May 5 13:21:09 2015 -0500

    Move and merge 'Function pointers' section from V1.md into AstSematics.md

[33mcommit bf5ab6a5af023d40adb57a4780d19ec8e1280658[m
Merge: 56579a7 25965a8
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 10:22:58 2015 -0500

    Merge pull request #45 from WebAssembly/downgrade-ptcs
    
    Move PTCs out of essential post-v.1 features

[33mcommit 56579a78d015bb946d4f7a38da1e10227394e5e9[m
Merge: c4f5257 ec5fa70
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 10:20:19 2015 -0500

    Merge pull request #43 from WebAssembly/add-backcompat
    
    Add backwards-compatibility section to BinaryEncoding.md

[33mcommit c4f52578abd6bf017d0594bbb29c7b4ad4ff7ebe[m
Merge: 4fdc9c2 a4fc811
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 10:14:19 2015 -0500

    Merge pull request #39 from WebAssembly/comma-and-conditional
    
    Add control-flow expressions to AstSemantics.md

[33mcommit a4fc811944009b72cd7a0a50bd53aa94745b4b22[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 10:43:50 2015 -0500

    Add control-flow expressions to AstSemantics.md

[33mcommit 4fdc9c2ef2d6954243894008f11256da70f431de[m
Merge: 6d8d02d bf1cc28
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu May 7 09:50:08 2015 -0500

    Merge pull request #42 from WebAssembly/add-i32-f32-f64-ops
    
    Add/tweak Int32/Float32/Float64 ops

[33mcommit bf1cc282b0c0d5e32d8927e5f902cf42a9972a78[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 11:25:18 2015 -0500

    Add/tweak Int32/Float32/Float64 ops

[33mcommit 25965a893cafc1282c10241ba22170eb5642974e[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 19:11:11 2015 -0500

    Move PTCs to FutureFeatures, elaborate on choices

[33mcommit 6d8d02dd3333ee67d4ab1f0d3192674eb1fca8b9[m
Merge: f701971 62f939d
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 18:47:34 2015 -0500

    Merge pull request #35 from WebAssembly/two-loops
    
    Describe 2 basic loop types

[33mcommit ec5fa70443b29cde38170015343055d717306210[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 17:45:45 2015 -0500

    Say what happens when a name is unknown

[33mcommit e912ffde2a5cddf065f953c26e7842211427cede[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 14:19:24 2015 -0500

    Update HighLevelGoals.md

[33mcommit 751fb579ca6efe105b0ada92c955ce34cd280ffe[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 14:11:59 2015 -0500

    Update V1.md

[33mcommit 7e3930dd0954f6997f5cbc952b95527f8b4464cd[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 14:10:46 2015 -0500

    Add 'Backwards Compatibility' section to BinaryEncoding

[33mcommit f70197194454c124199dabce2bad8bc3c5ec4648[m
Merge: fc929fb 81a5728
Author: JF Bastien <github@jfbastien.com>
Date:   Wed May 6 11:54:10 2015 -0500

    Merge pull request #40 from WebAssembly/add-reinterpret
    
    Add int<->float reinterpretation

[33mcommit 81a5728765b7b07a46d046d642c93d37530f27fe[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 11:03:06 2015 -0500

    Add int<->float reinterpretation

[33mcommit fc929fbe4b066e3aa03898a328b8f12d7ff66b96[m
Merge: 189a925 d12c093
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 10:44:51 2015 -0500

    Merge pull request #37 from WebAssembly/add-literals
    
    Add literals to AstSemantics

[33mcommit 189a925ecd190957a2caf0d0884407d98f6a466d[m
Merge: 911917c d2acacf
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 10:44:10 2015 -0500

    Merge pull request #36 from WebAssembly/add-switch
    
    Add Switch to AstSemantics

[33mcommit d12c09301e8929e9d03dcc3bf554b48ce5050a64[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 10:05:11 2015 -0500

    Update AstSemantics.md

[33mcommit d2acacf3f6b6ad6b965725f5025aa19514ee9cc6[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed May 6 09:53:55 2015 -0500

    Add Switch to AstSemantics

[33mcommit 62f939dc47da775ea936a540df6a1952d16eea2f[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Tue May 5 16:28:06 2015 -0700

    describe 2 basic loop types

[33mcommit 911917c6f1a1339375d8a81efbe9322d641bd10d[m
Merge: 29c5973 7aa418a
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue May 5 16:15:27 2015 -0500

    Merge pull request #28 from WebAssembly/load-store-with-offset
    
    Add (Load,Store)WithOffset

[33mcommit 7e924a58b089ffda1cbacac4715bc5bbcaf56b0d[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Tue May 5 07:16:47 2015 -0700

    Discuss optimization for JITs.
    
    Expand the Platform-independent Just-in-Time compilation section to
    mention the challenge of providing optimization for the simple JIT use
    case.

[33mcommit 29c5973ff6a33e863a302f7f03eb5cfd6f06e314[m
Merge: 2a98b08 5bf9292
Author: Luke Wagner <mail@lukewagner.name>
Date:   Tue May 5 08:16:57 2015 -0500

    Merge pull request #22 from WebAssembly/text-format
    
    Clarify the equivalence between the binary and text format

[33mcommit 5bf929243cd8f2ce0388bccc4009ace8e01d18b2[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Sat May 2 13:47:03 2015 -0700

    clarify the purpose of the binary format, and its equivalence to the text format

[33mcommit 2a98b08e4c252a6b4729660975e435d101726330[m
Merge: 59f06dc 3ec7cf8
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 4 19:18:29 2015 -0500

    Merge pull request #23 from WebAssembly/heapster-chic
    
    Clarify about JS-heap-sharing in V1 and clarify polyfill-not-100%-precise in high-level-goals.

[33mcommit 59f06dcabf413054704027438df6614020f697f3[m
Merge: da16032 6410ea4
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 4 15:45:33 2015 -0700

    Merge pull request #26 from WebAssembly/no-frem
    
    Remove floating-point mod, and rename integer mod to remainder.

[33mcommit 3ec7cf8fc09a59a0bee66329783dd1ca7df93bc4[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Mon May 4 11:10:20 2015 -0700

    mention polyfill limitations

[33mcommit 2d779d9439896e4349ce45e083c7babc56716084[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Sat May 2 14:11:10 2015 -0700

    clarify heap sharing to JS

[33mcommit 7aa418a5657c07a65fe3f852e264a0e94ba8615a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 4 11:23:08 2015 -0500

    Propose (Load,Store)WithOffset

[33mcommit 6410ea4f9a9a62f1c7826c43edb2ee8b90758422[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Mon May 4 08:08:28 2015 -0700

    Remove floating-point mod, and rename integer mod to remainder.
    
    Floating-point mod is not in IEEE-754, not implemented in any hardware,
    and not in C or C++ or similar languages. It's in asm.js, but probably
    only because it's in JS, and it's in JS possibly only because JS Numbers
    are intended to be somewhat usable as integers. Floating-point mod does
    have use cases, however they can adequately be serviced by the math
    library, in whatever form that ends up taking.
    
    This patch also renames integer mod to remainder, since "signed modulus"
    in particular is a non-obvious way to describe the mathematic operation
    it performs.

[33mcommit da16032dd83d67a98b57490fe993597d4f1fe8bd[m
Merge: 5df3554 367c654
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 4 09:40:27 2015 -0500

    Merge pull request #25 from WebAssembly/add-ast-semantics
    
    Add initial semantics for AST.

[33mcommit 5df35548b39d91e695c49a8c48564d5718a21049[m
Merge: 9712e4f 1625dec
Author: Luke Wagner <mail@lukewagner.name>
Date:   Mon May 4 09:38:46 2015 -0500

    Merge pull request #24 from WebAssembly/bnjbvr-fix-simd-proposal-link
    
    Fix SIMD proposal link in EssentialPostV1Features

[33mcommit 367c654fe964c741d1e1a9b2203a9002561a2ff0[m
Author: Ben L. Titzer <titzer@google.com>
Date:   Mon May 4 14:47:38 2015 +0200

    Add initial semantics for AST.

[33mcommit 1625dec9fc0aef70f74da56459ff9e66be64f0af[m
Author: Benjamin Bouvier <public@benj.me>
Date:   Mon May 4 11:29:47 2015 +0200

    Fix SIMD proposal link in EssentialPostV1Features

[33mcommit 9712e4ff672fb128961d1a0591488febd412fc92[m
Merge: a19e4cc 1be96c0
Author: JF Bastien <github@jfbastien.com>
Date:   Sun May 3 12:27:41 2015 -0700

    Merge pull request #21 from WebAssembly/reloop
    
    Relooper clarifications

[33mcommit 1be96c05f60b5c3463c6a3546707fedf5c0306c4[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Sat May 2 12:55:08 2015 -0700

    relooper clarifications

[33mcommit a19e4ccf9c250cc730afa9941944f377a7c87dcd[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 18:41:13 2015 -0500

    Link HighLevelGoals.md to V1.md

[33mcommit 085de52367911ecae909b74c1dab49075899f2cf[m
Merge: c2ba24e 7f16bc2
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 18:39:59 2015 -0500

    Merge pull request #20 from WebAssembly/binary-encoding-wip
    
    Binary encoding doc

[33mcommit 7f16bc2903bc7c99be4d9258b1222eaddf06f61a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 18:38:17 2015 -0500

    Update BinaryEncoding.md

[33mcommit 7d98a1f3ae6b329f965850745c859eb4f0bb99b5[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 21:49:25 2015 -0500

    Create AstSemantics.md

[33mcommit f7114571dae9cd265a57258955e7e883d6a67f09[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 21:48:25 2015 -0500

    Add links to new docs in v1 doc

[33mcommit 05436f1359a3d16559ee4032c9fb5bc98ab7ded4[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 21:41:30 2015 -0500

    Start working on BinaryEncoding.md
    
    Just throwing up an outline, still a WIP.

[33mcommit c2ba24ee166134595b05c50188b33de82bcba645[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 15:53:15 2015 -0500

    Add source maps as a feature

[33mcommit b1c488d62ab533e79432bf9b4d66716c0ce3e4b5[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 15:40:37 2015 -0500

    s/would expect/might find/ to encourage convergence

[33mcommit fd939b94245c2577a9a17a61ea2ba6ec3ff4136a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 15:30:39 2015 -0500

    Tidy up wording and in V1.md

[33mcommit 739e307e00e190212644d339a8c0dc80815f2c8e[m
Merge: 751cba5 9c6420c
Author: JF Bastien <github@jfbastien.com>
Date:   Fri May 1 13:14:58 2015 -0700

    Merge pull request #19 from WebAssembly/add-fps
    
    Add external function pointers

[33mcommit 9c6420ce0f6502e10ffeb2415722a2df89ae3c11[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Fri May 1 11:23:38 2015 -0700

    mention dynamic linking of non-wasm code

[33mcommit 751cba5e82b4fcaf649a5e569ce02d5d4ea84898[m
Merge: cc44fd8 d91f4a2
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 13:41:31 2015 -0500

    Merge pull request #12 from WebAssembly/fill-in-int64
    
    fill in int64

[33mcommit cc44fd8945df107c3b3ad35c2257e2ce25f3f5b7[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 13:31:01 2015 -0500

    wich

[33mcommit 1ec4214ad66a0898906800b8cf1366ee12f324b9[m
Merge: fdfa71c 741b02e
Author: JF Bastien <github@jfbastien.com>
Date:   Fri May 1 10:26:38 2015 -0700

    Merge pull request #18 from WebAssembly/move-tooling-link
    
    Move tooling link.

[33mcommit 741b02ecc492b05449191da40fd1e00e77e71a64[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri May 1 10:26:00 2015 -0700

    Move tooling link.

[33mcommit d91f4a2a4537b5ba6181793224f7c006b2f6b07e[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 11:29:18 2015 -0500

    Split of 4GiB heap feature

[33mcommit fdfa71c9fb19d33e85d390deeae2cbdb7ab3eacb[m
Merge: d12c3bd fd09da2
Author: JF Bastien <github@jfbastien.com>
Date:   Fri May 1 09:22:30 2015 -0700

    Merge pull request #17 from WebAssembly/link-tooling
    
    Link tooling.

[33mcommit fd09da2f33bf556ce45ec482ecb2f6d0e5fe7426[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri May 1 09:22:05 2015 -0700

    Link tooling.

[33mcommit 94fdc6674635ebe687185f50b5f6a20281eaab39[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 11:18:36 2015 -0500

    address comments

[33mcommit d12c3bdf3632055ca22be7b44a4f9f71b467f332[m
Merge: b9eb596 8e0805c
Author: JF Bastien <github@jfbastien.com>
Date:   Fri May 1 09:16:04 2015 -0700

    Merge pull request #15 from WebAssembly/tooling
    
    Add a tooling section.

[33mcommit 8e0805c8bdcdd086eb8079a3b3350adca09e7bac[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri May 1 09:15:38 2015 -0700

    Move multiprocess to end.

[33mcommit b9eb596026132ac504c1f98d3fdf5be6334b7898[m
Merge: a063e16 0c52058
Author: JF Bastien <github@jfbastien.com>
Date:   Fri May 1 09:07:18 2015 -0700

    Merge pull request #16 from WebAssembly/expand-on-ptcs
    
    Expand on PTCs

[33mcommit 0c520589e309341d2e83a9bf6a351ad0d797808c[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 11:03:54 2015 -0500

    Expand on PTCs

[33mcommit 76d99413d077529a21be6f7bd9643f2cab9bf84e[m
Author: JF Bastien <jfb@chromium.org>
Date:   Fri May 1 08:45:32 2015 -0700

    Add a tooling section.

[33mcommit a063e1692efb69c14a6448e3d1ba63fcbcced4ff[m
Merge: cadec6e c648a04
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 10:42:10 2015 -0500

    Merge pull request #13 from WebAssembly/fill-in-SIMD
    
    Fill in SIMD

[33mcommit c648a04e8dd87a33fa6a63381846aaa16cc1a8ef[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 10:41:11 2015 -0500

    Address comments

[33mcommit cadec6e065a579371f0938d6791f2c5ad45d7977[m
Merge: 1716058 ad62ada
Author: JF Bastien <github@jfbastien.com>
Date:   Fri May 1 08:30:35 2015 -0700

    Merge pull request #14 from WebAssembly/fill-in-thread-details
    
    fill in more details about threads

[33mcommit ad62ada0defada3655ab7e6a2daff8ebe5db7635[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 10:28:16 2015 -0500

    fill in more details about threads

[33mcommit 247be80590029da3da1add9705dfe6efe76079ee[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 10:02:39 2015 -0500

    Fill in SIMD

[33mcommit 1716058d1ce559d2187d2d55a097289ec109f6a1[m
Merge: dae7a37 3c2f0e1
Author: JF Bastien <github@jfbastien.com>
Date:   Fri May 1 07:59:01 2015 -0700

    Merge pull request #10 from WebAssembly/tweak-dynamic-linking
    
    "linking" instead of "loading"

[33mcommit 9bdee79ef1c30090d59366043ff6db0a023a7185[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Fri May 1 09:57:01 2015 -0500

    fill in int64

[33mcommit dae7a37fc4a54654753769e789c265de045ccaf5[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 21:00:10 2015 -0500

    Remove low-level encoding details from the code section

[33mcommit 3c2f0e18b3c9803c27153ba6c628516e45c742cc[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 19:06:03 2015 -0500

    Update FutureFeatures.md

[33mcommit 267a25670ebab86105345770a1ed1621e0e9011b[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 19:01:47 2015 -0500

    Update V1.md

[33mcommit 070f56c5c59030e2d735973f3e10dbe2c87366db[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 19:00:51 2015 -0500

    Fill in V1.md
    
    Fill in with the discussion from the previous GDoc to start discussion here.

[33mcommit d5093ea4f7804f88f56a83e9139456644a0fd195[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 13:41:07 2015 -0500

    Update EssentialPostV1Features.md

[33mcommit 31bbe91dcde90a488ae1b4e5c28bcd9602751ecc[m
Merge: 665e0d2 319f547
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 13:37:31 2015 -0500

    Merge pull request #9 from WebAssembly/setjmp-note
    
    clarify setjmp/longjmp limitations when implemented using exceptions

[33mcommit 319f547467938026dd3ff49d224ecd8198af5f7d[m
Author: Alon Zakai <alonzakai@gmail.com>
Date:   Thu Apr 30 11:14:54 2015 -0700

    clarify setjmp/longjmp limitations when implemented using exceptions

[33mcommit 665e0d2008b6c8f4f6eebd05e85117201200de5c[m
Merge: b2ace05 2a4ab09
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 11:06:39 2015 -0700

    Merge pull request #8 from WebAssembly/ptc
    
    Clarify tail call and PTC.

[33mcommit 2a4ab09d3dbe63589132eec2a6570b2ff15339cc[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Apr 30 11:04:22 2015 -0700

    Clarify tail call and PTC.

[33mcommit b2ace05673133a948022d989b635ecba003d9248[m
Merge: 3a1c4f4 3df570c
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Apr 30 10:56:29 2015 -0700

    Merge pull request #7 from WebAssembly/float-typo-and-more
    
    Fix a typo, and more!

[33mcommit 3df570cbc63d0a363a216baf6f55633a401c0bdc[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 10:52:40 2015 -0700

    Fix a typo, and more!

[33mcommit 3a1c4f45822fda5eeca325545111183b2fb11c80[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 12:35:57 2015 -0500

    GH doesn't like irc:// links I guess...

[33mcommit 8be602f39b64612b063ff06051e8c988d62bace3[m
Merge: ff780ea 8f2799b
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Apr 30 10:28:14 2015 -0700

    Merge pull request #6 from WebAssembly/tweak-highlevelgoals
    
    Wording tweaks to HighLevelGoals.md

[33mcommit ff780eacb434c42560d80fc8dc657ca2d936eab0[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 12:19:58 2015 -0500

    Add spaces so it formats nicely

[33mcommit 8f2799b38d87a0fffd27a7c47386acef6394aa1f[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 12:15:17 2015 -0500

    Wording tweaks to HighLevelGoals.md

[33mcommit dd9f541a2d82626907d64a53a8d85d7e349a3e8a[m
Merge: 9acba53 c21bf63
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 10:14:16 2015 -0700

    Merge pull request #5 from WebAssembly/linking-bikeshed
    
    This is the web. It likes links.

[33mcommit c21bf63b6e857bff31743184daa123cb43950efc[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Apr 30 10:09:44 2015 -0700

    This is the web. It likes links.

[33mcommit 9acba53cdae9032a5d661664c2670a7dfe886f3a[m
Merge: 60303f2 f9917a8
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Apr 30 10:01:48 2015 -0700

    Merge pull request #4 from WebAssembly/moar-features
    
    Add a few features.

[33mcommit f9917a82bb4d9240960969838fde13e64ad72bd5[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Apr 30 09:59:08 2015 -0700

    Add a few features.

[33mcommit 60303f2bf99fcf72b9ba25da099a801dd142ae8e[m
Merge: 878f114 156cb20
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 09:52:19 2015 -0700

    Merge pull request #3 from WebAssembly/essentialpostv1-future
    
    Got an lgtm from luke on irc.

[33mcommit 156cb20fdf115edfbd688c77f11eb890596e62de[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 09:48:40 2015 -0700

    Remove irreducible control flow.
    
    The functionality is presently expected to be covered by Proper Tail Calls.

[33mcommit 60dff8ed5e91931ad9e0b9415cbf67a0b703a0fe[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 09:44:15 2015 -0700

    Move JIT support to FutureFeatures.md.

[33mcommit 8307058313e61ed9a50cdbaa313ff76f47db0b04[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 09:44:03 2015 -0700

    Remove features from EssentialPostV1Features.md covered in FutureFeatures.md.

[33mcommit 169f3f0c2af3e863634010a3dd230eeca2cd6ba1[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 09:43:51 2015 -0700

    Move the note about prioritizing features based on developer feedback to FutureFeatures.md.

[33mcommit 6f72e6053c63f1ab96fb71fc427afd34ff689e2f[m
Author: Dan Gohman <sunfish@mozilla.com>
Date:   Thu Apr 30 09:43:40 2015 -0700

    Clean up accidential duplication of "extra math" features.
    
    And clarify the portability situation for "extra math" features.

[33mcommit 878f1149991414720edfc0a32bb77e9ffeb6b251[m
Merge: 16687d3 b289380
Author: JF Bastien <github@jfbastien.com>
Date:   Thu Apr 30 09:22:45 2015 -0700

    Merge pull request #2 from WebAssembly/high-level-goals-2
    
    Update high-level goals, and add post-v1 features.

[33mcommit b28938050bfd04a493860a00c5c0498179f4228f[m
Author: JF Bastien <jfb@chromium.org>
Date:   Thu Apr 30 09:21:46 2015 -0700

    Update high-level goals, and add post-v1 features.

[33mcommit 16687d30a0859b219c925c3630d195c0cf30efee[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 11:12:54 2015 -0500

    Create FutureFeatures.md

[33mcommit 000bdba47d01bbbd7f4ccf673a5af56320a90535[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 10:52:09 2015 -0500

    Update EssentialPostV1Features.md

[33mcommit 31c7927d610ed25c3a0ca3a7f573209e42040a42[m
Author: Benjamin Bouvier <public@benj.me>
Date:   Thu Apr 30 17:51:06 2015 +0200

    Change v1 link in EssentialPostV1Features.md

[33mcommit a899cfdc03f85e48709df7f51f5607408352f711[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 10:43:42 2015 -0500

    Rename v1.md to V1.md

[33mcommit ccc77a33ec6a8f10ca7655b83be96422377f103a[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 10:43:25 2015 -0500

    Create EssentialPostV1Features.md
    
    Just a skeleton to start discussion.

[33mcommit 11c7a46db93d990355eb6d7e91874b636b3157ce[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Thu Apr 30 10:39:07 2015 -0500

    Create v1.md
    
    Just throwing up a skeleton

[33mcommit 2e0b8835931d116d8391b897c95adc7f75aa6af2[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Apr 29 15:30:06 2015 -0500

    Update README.md

[33mcommit 51c6a84aa210cca51db7d8963b442375df103bfe[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Apr 29 15:29:20 2015 -0500

    Update HighLevelGoals.md

[33mcommit 12ee148fb5cfa33331dbffadae06752b1759a7bf[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Apr 29 15:11:28 2015 -0500

    Create HighLevelGoals.md
    
    This high-level design goal list has been passed around and generally agreed upon, so I'll put it up.  Of course we can continue to discuss and iterate in issues.

[33mcommit 1aff6a5eacd342f3d0e6ce37d2a7a2d2c7e5ad10[m
Author: Luke Wagner <mail@lukewagner.name>
Date:   Wed Apr 29 12:36:05 2015 -0500

    Initial commit
