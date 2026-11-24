# Create Issue — µOS++ IIIe Rules

The rules of the `create-github-issue` skill that are specific to this
project. They extend `SKILL.md`, and take precedence where they differ.

## Older Issues

The issues up to #155 have titles in the form
`fix(<area>): <summary>`, and bodies with `Problem:`, `Change:`, and
`Test:` paragraphs, and a `Fixed on <date> in <hash>` line appended to
the body. They were written before the skill existed; do not copy
their form. New issues follow `SKILL.md` (a title that states the
problem, a short prose body, and the fix recorded as a comment by the
`github-commit` skill).

## Labels

The repository has no `documentation` label; use `bug` or
`enhancement`, as `SKILL.md` describes.

## Milestones

- The version milestones are named after the version without a `v`
  prefix (`7.2.0`); the older `v6.3.*` ones are closed.
- `Next release` and `Time permitting` are older placeholders; do not
  propose them, unless the user says so.

## Body

- Name the port when the problem depends on it (Cortex-M or POSIX),
  and the test configuration that shows it (for example
  `qemu-cortex-m0-cmake-gcc-debug`).
- For a race or a lost wake-up, describe the interleaving in one
  sentence (which thread or interrupt does what, in which order), since
  it is the evidence; quote the probe only when it is short.
- When the fix would change the observable behaviour of the RTOS, say
  so in the proposed change: it needs a `**Behaviour change:**` entry
  in `CHANGELOG.md`.
