---
name: review-and-fix
description: Code review and fix workflow for the µOS++ IIIe repository - write a review document (maintenance/docs/CODE-REVIEW-N.md) of a branch or change, then fix the findings one at a time with a proposal, deterministic tests with a negative check, a GitHub issue and a commit each, keeping the review document up to date. Use it whenever the user asks to review changes or a branch, asks "how do you suggest to fix #N", says "apply it", "create github issue for #N and commit", "commit without a github issue", or "anything else left?", or otherwise works through review findings in this project, even if the word "skill" or "review" is not used.
---

# µOS++ IIIe review and fix workflow

This skill captures a workflow that was used to review a large change set
(`xpack` → `xpack-development`) and to fix its findings one by one. It has
two phases: writing the review, then a fix loop driven by short user
prompts. The user drives the pace; each step ends by reporting and, when a
decision is needed, asking.

Read `CLAUDE.md` first (build, test, formatting, port contract, coding
conventions, including the portability rule). For details that are not
needed every time (issue body template, review document skeleton, test
patterns, pitfalls), read `references/details.md` when you reach that step.

## Phase 1 - the review document

1. Agree the scope (for example `git diff xpack...xpack-development`, a PR,
   or a folder) and the review depth. `maintenance/` is never reviewed,
   but the review document itself is written there, as
   `maintenance/docs/CODE-REVIEW-<N>.md`.
2. Look for real bugs added or made worse by the change: scheduler and
   thread state, synchronisation objects, startup, C API sync, ports.
   Check every claim against the code and against both ports
   (`micro-os-plus-iii-cortexm`, `micro-os-plus-iii-posix-arch`), the
   documentation and the tests.
3. Write one section per finding (template in `references/details.md`):
   title, **File**, **Status**, Problem, Scenario, Documentation and
   tests, Port behaviour, Suggested fix. Add a summary table with a
   severity per finding (important, pre-existing, SMP only, minor,
   narrow, decision, docs, nit) and a suggested fix order in phases
   (small and contained first, decisions next, scheduler core last, one
   at a time with tests in between).
4. Keep the review document uncommitted until the user asks.

## Phase 2 - the fix loop

The user works through findings with a fixed set of prompts. Respond to
each as below.

### "how do you suggest to fix #N?"

Do not just repeat the review's suggestion; re-derive it.

- Re-read the current code: earlier fixes may have changed it, and the
  review may be wrong, overstated or understated (for example a window
  that is narrower than described, or the same bug in other objects).
  Say so explicitly; this is where most of the value is.
- Check both ports, the `smp` branch when relevant, and what the
  documentation promises.
- Give one recommendation (not a survey), with a short code sketch,
  and say what it does not change (ports, C API, data layout) or, if it
  does, why.
- Call out behaviour changes; those need a `CHANGELOG.md` entry
  ("**Behaviour change:** ...") and the user's agreement.
- Propose the tests: deterministic where possible (thread priorities,
  scheduler lock, self-interrupt; see `references/details.md`), a stress
  test when a race cannot be forced, or regression-only tests when
  nothing can reach the case, saying so honestly. Plan a negative check.
- End with "Shall I apply it?". If the user adds constraints or corrects
  you, fold them in and re-propose rather than arguing; if a choice is
  genuinely theirs (naming, layout, two valid designs), ask with concrete
  options.

### "yes, apply it"

1. Implement the fix. Keep trace and instrumentation calls, the
   `in_handler_mode()` checks, British English, "folder" not "directory",
   the µOS++ licence header in new files, and the portability rule.
2. Add the tests (usually in `tests/sources/rtos-apis/src/test-cpp-api.cpp`).
3. **Negative check**: show the new test fails without the fix. Save the
   fixed file to the scratchpad, restore the original from `HEAD` (or
   from the previous fix), make test failures non-fatal if one failure
   would hide the others, build and run natively, then restore the
   fixed files. If the test passes without the fix, the test is wrong
   (it happened: a waiter that never actually blocked) - fix the test.
4. Format only the touched files:
   `xpacks/.bin/clang-format --style=file:maintenance/config/.clang-format -i <files>`
   (cmake files: `cmake-format --config-file maintenance/config/.cmake-format.py --in-place`).
5. Run the suites: `xpm run test-native-cmake-clang -C tests`,
   `xpm run test-qemu-cortex-m7f-cmake -C tests`,
   `xpm run test-qemu-cortex-m0-cmake -C tests` (add M3 / M4F and
   `test-native-cmake-gcc` for wide changes). Long runs may exceed the
   tool timeout; run them in the background.
6. For refactors that must not change behaviour, compare release
   binaries before and after with `scripts/hash-builds.sh` (the CMSIS
   validator embeds `__TIME__`, so only a few timestamp bytes may
   differ there).
7. Update the review document: the **Status** line ("fixed (not
   committed) ..."), sections "Analysis before fixing" (what the review
   got right or wrong), "Fix applied", "Verification" (including the
   negative check result), and the summary table / fix order.
8. Report concisely: what changed, test results, anything that could
   not be verified, behaviour changes. Do not commit.

### "create github issue for #N and commit"

1. Write the issue body to the scratchpad (template in
   `references/details.md`: Problem / Change / Test and results) and
   create it:
   `gh issue create --title "fix(<area>): <summary>" --label bug --milestone <version> --assignee <user> --body-file <file>`.
   Use the current milestone (ask the first time).
2. Commit **only the code files** of this fix (sources, headers, tests,
   `CHANGELOG.md` when changed); never the review document, the user's
   own edits (for example `tests/package.json`), or other fixes. If two
   fixes share a file, stage a fix-only version with
   `git hash-object -w` + `git update-index --cacheinfo`, after building
   and testing that exact state.
   Subject: `<file or area>: <short summary> (#<issue>)`; body: a few
   bullets; end with the attribution trailer the system prompt asks for.
3. Append `Fixed on <YYYY-MM-DD> in <full sha> (\`<branch>\`).` to the
   issue body (`gh issue edit <n> --body-file <file>`). Issues stay
   open until the release.
4. Update the finding's **Status** line and the fix-order entry in the
   review document with the short SHA and the issue number.
5. Never push unless asked.

### "commit without a github issue"

For small fixes (typos, version bumps, test-only fixes, documentation):
commit the named files only, short subject, attribution trailer.

### "check the same issue in the other packages / fix it there too"

Survey the sibling repositories (ports, `xPacks/*`) for the same pattern,
report a table (package, code, result), and fix only where asked. Other
repositories have their own conventions (branch, commit style, issue
tracker; a fork may have issues disabled and no push rights) - check
before committing, and report what cannot be pushed.

### "anything else left?"

Check `git status`, `git rev-list --count origin/<branch>..HEAD`, stashes,
and the review document, then list what is left, grouped: decisions
for the user (push, committing the review, other branches, release
steps) and things noticed but not part of the review. Offer the next
step instead of starting it.

## Things that went wrong before (avoid them)

- A fix proposal that copied the review's suggestion without checking
  the code missed that the bug was elsewhere too, or narrower.
- A guard using `__ARM_EABI__` was rejected: the code must stay portable
  (see the portability rule in `CLAUDE.md`).
- Stashing for a baseline build swept in a file the user was editing at
  the same time; always stash with an explicit list of your own files.
- Linked dependencies (`xpm link`) can point at sibling repositories on
  the wrong branch (for example `smp`); when tests fail in CMake before
  compiling, check the links and branches first.

More pitfalls (zsh quoting, compiler warnings, test design) are listed in
`references/details.md`.
