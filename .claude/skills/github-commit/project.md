# Commit — µOS++ IIIe Rules

The rules of the `github-commit` skill that are specific to this
project. They extend `SKILL.md`, and take precedence where they differ.

## Older Commits

The commits up to `82819dc3` refer to the GitHub issue at the end of
the subject (`os-condvar.cpp: timed_wait() result from what woke the
thread (#146)`), with a body of bullets. They were written before the
skill existed; do not copy their form. New commits use the
`#N: <summary>` subject and the prose body of `SKILL.md`.

## Before Committing

- Run the tests listed under "Tests" in `.github/skills/project.md`,
  for the ports that the change affects; a change to the core runs
  both the native and the QEMU tests.
- A user-visible change, or a behaviour change, has its `CHANGELOG.md`
  entry in the same commit (under the dated heading of the day, at the
  top; `**Behaviour change:** ...` for the changes of behaviour).
- A change to the port contract is committed together with the
  matching changes in the port repositories (each in its own
  repository, with its own conventions); say which port commits it
  needs.
- When two fixes share a file, and only one of them is committed, stage
  only its part (`git add -p` is interactive and not available; write
  the fix-only version of the file, build and test that exact state,
  and stage it with `git hash-object -w` and
  `git update-index --cacheinfo`), and tell the user.

## Review Documents

`maintenance/docs/CODE-REVIEW-1.md` uses its own format for the fixed
items (see "Review Documents" in `.github/skills/project.md`); update
its `**Status:**` line and its summary table instead of adding the
`(fixed)` tag.
