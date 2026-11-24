# Agent Skills — µOS++ IIIe

<!--
DO NOT EDIT!
Automatically generated from template file: npm-packages-helper/templates/common/.github/skills/README-liquid.md.
-->

This folder holds the agent skills of the project, shared by GitHub
Copilot (through `.github/copilot-instructions.md`) and Claude Code.
The files are in `.claude/skills`, where Claude Code looks for them;
`.github/skills` is a symbolic link to that folder, so that the paths
`.github/skills/...` used by the instruction files and the skills are
valid. Each skill is a sub-folder with a `SKILL.md` file, whose front matter (`name`,
`description`) tells the agent when to use it.

Most of the files are generated from the templates of the
`npm-packages-helper` package, and are regenerated with
`npm run generate-top-commons`; the rules specific to this project are
kept in separate files, which are never regenerated. This file
explains which is which, and where each kind of rule goes.

## The Files

| File | Origin | Regenerated |
| --- | --- | --- |
| `README.md` (this file) | template | yes |
| `review-common.md` | template | yes |
| `<skill>/SKILL.md`, `<skill>/scripts/*` | template | yes |
| `project-defaults.md` | template | yes |
| `project.md` | template, created once | no |
| `<skill>/project.md` | the project, optional | no |
| `<skill>/SKILL.md` of a skill not in the template | the project | no |

- The generated files start (after the front matter, in the `SKILL.md`
  files) with a `DO NOT EDIT!` comment, which names their template. An
  edit made in them is lost at the next regeneration.
- `review-common.md` holds the rules shared by the review skills
  (judging, tagging, and numbering the findings, the names and the
  format of the report files, and acting on an item); it is not a
  skill.
- `project-defaults.md` holds the default values of the project facts
  that several skills use, one section for each heading listed below,
  rendered for the organisation and the project (for example, the
  tests, or the documentation, depend on whether the project has a
  `tests/` or a `website/` folder).
- `project.md` holds only what differs in this project from the
  defaults. It is created from a short stub only if it does not exist;
  after that, it belongs to the project.
- `<skill>/project.md`, for example `code-review/project.md`, holds the
  rules of a single skill. The template never creates it; a skill reads
  it only if it exists.
- A skill that exists only in this project (a folder whose `SKILL.md`
  has no `DO NOT EDIT!` comment) belongs to the project, and is never
  touched by the generator.

## The Skills of the Template

- `code-review` — a review of the style, documentation, and
  correctness of the C++ code, or of a change;
- `consistency-check` — the consistency of the names, the
  configurations, the READMEs, and the instruction files;
- `create-github-issue` — draft and open GitHub issues;
- `create-javascript-mjs` — write a new `.mjs` script;
- `design-review` — a review of the design of the C++ API, from the
  code alone (also used for an "API review");
- `doxygen-review` — verify and fix the Doxygen comments; its rules
  apply to every edit of the code;
- `github-commit` — prepare a commit, and the closing comment of the
  GitHub issue;
- `rewrite-bash-to-javascript` — convert a `.sh` script to `.mjs`;
- `xcdl` — how `xcdl-package.jsonc` generates the CMake and meson
  files.

## Where to Put a Rule

When a rule specific to this project must be added or changed:

1. If several skills use it (a test command, how to build a probe, a
   generated file, a label, the milestones, the review documents, the
   public contract, the documentation to update, the short form of the
   paths), and it differs from `project-defaults.md`, write it in
   `project.md`, under the matching heading; do not repeat a default.
2. If only one skill uses it (a restriction of the design review, an
   exception to the Doxygen rules, a decision about the API pages, an
   example), write it in `<skill>/project.md`, creating the file if
   needed.
3. If the rule is not specific to this project, and should apply to
   the other projects as well, change the template instead (see
   "Changing the Common Rules").

Never edit a generated `SKILL.md`, `review-common.md`, or this file.

## Precedence

The files are read from the most general to the most specific, and,
where they disagree, the more specific one wins:

1. `<skill>/SKILL.md` (and `review-common.md`, for the review skills);
2. `project-defaults.md`;
3. `project.md`, whose heading extends the default of the same
   heading, or overrides it where the two disagree (a heading that says
   "None", or "Not applicable", removes the default);
4. `<skill>/project.md`.

## The Headings of `project.md`

The generated files refer to these headings by name, so do not rename
them (a heading that is absent from `project.md` keeps its default in
`project-defaults.md`, and a heading may say "None" to remove it);
other headings may be added, for example for the extra rules about the
milestones:

- **Tests** — the tests to run after major and after minor changes;
- **Probes** — how to build and run a probe program;
- **Generated Files** — the files generated by the project tools, from
  what, and how to regenerate them;
- **Files Not to Commit** — the files that are never committed;
- **Review Documents** — the documents in `maintenance/docs/` that list
  findings, and the format of the notes of the fixed items;
- **Breaking Changes** — the public contract, whose changes are
  **[breaking]**;
- **Documentation Updates** — which documentation is updated by hand,
  and which is generated;
- **Paths** — the short forms of the paths allowed in the reports and
  the issues.

## Changing the Common Rules

The templates are in the `npm-packages-helper` package:

- `templates/common/.github/skills/` — the files regenerated every
  time (the sub-folders named `_micro-os-plus/` and `_xpack/` hold the
  files of a single organisation; the folder name is removed when the
  file is generated);
- `templates/first-time/.github/skills/project-liquid.md` — the stub of
  `project.md`; the defaults are in
  `templates/common/.github/skills/project-defaults-liquid.md`.

The templates are Liquid files (`-liquid` in their names); the
differences between the organisations and between the projects are
written as Liquid conditions on the configuration of the project (for
example `githubProjectOrganization`, `hasFolderWebsitePackage`,
`topConfig.useDoxygen`).

To apply a change, with `node_modules/@xpack/npm-packages-helper`
linked to a writable checkout (`npm run npm-link-helper`), run in the
top folder of each project:

```sh
npm run generate-top-commons -- --dry-run
npm run generate-top-commons
```

The generator also refreshes the other top-level files of the project,
and `maintenance/config/top-templates.json`; review all the changes
before committing them.

## Adding, Renaming, or Removing a Skill

- A skill that only this project needs is created by hand, in its own
  folder, without a `DO NOT EDIT!` comment; do not give it the name of a
  skill of the template, which would overwrite it.
- The generator does not delete files: when a skill is removed or
  renamed in the template, remove the old folder by hand (after moving
  its `project.md`, if any, to the new folder).
