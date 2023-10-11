[![GitHub issues](https://img.shields.io/github/issues/micro-os-plus/micro-os-plus-iii.svg)](https://github.com/micro-os-plus/micro-os-plus-iii/issues/)
[![GitHub pulls](https://img.shields.io/github/issues-pr/micro-os-plus/micro-os-plus-iii.svg)](https://github.com/micro-os-plus/micro-os-plus-iii/pulls)

# Maintainer info

## Project repository

The project is hosted on GitHub:

- <https://github.com/micro-os-plus/micro-os-plus-iii.git>

To clone the stable branch (`xpack`), run the following commands in a
terminal (on Windows use the _Git Bash_ console):

```sh
rm -rf ~/Work/micro-os-plus/micro-os-plus-iii.git && \
mkdir -p ~/Work/micro-os-plus && \
git clone \
  https://github.com/micro-os-plus/micro-os-plus-iii.git \
  ~/Work/micro-os-plus/micro-os-plus-iii.git
```

For development purposes, clone the `xpack-develop` branch:

```sh
rm -rf ~/Work/micro-os-plus/micro-os-plus-iii.git && \
mkdir -p ~/Work/micro-os-plus && \
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/micro-os-plus-iii.git \
  ~/Work/micro-os-plus/micro-os-plus-iii.git
```

## Prerequisites

A recent [xpm](https://xpack.github.io/xpm/), which is a portable
[Node.js](https://nodejs.org/) command line application.

## How to make new releases

### Release schedule

There are no fixed releases, the project aims to follow the upstream releases.

### Check Git

In the `micro-os-plus/micro-os-plus-iii` Git repo:

- switch to the `xpack-develop` branch
- if needed, merge the `xpack` branch

No need to add a tag here, it'll be added when the release is created.

### Increase the version

Update the`package.json` file; add an extra field in the
pre-release field, and initially also add `.pre`,
for example `7.0.0-pre.1`.

### Fix possible open issues

Check GitHub issues and pull requests:

- <https://github.com/micro-os-plus/micro-os-plus-iii/issues/>

and fix them; assign them to a milestone (like `7.0.0`).

### Update `README-MAINTAINER.md`

Update the following files to reflect the changes
related to the new version:

- `README-MAINTAINER.md`
- `README.md`

### Update `CHANGELOG.md`

- open the `CHANGELOG.md` file
- check if all previous fixed issues are in
- add a new entry like _* v7.0.0_
- commit with a message like _prepare v7.0.0_

### Push changes

- commit and push

### Commit the new version

- select the `xpack-develop` branch
- commit all changes
- `npm pack` and check the content of the archive, which should list
  only `package.json`, `README.md`, `LICENSE`, `CHANGELOG.md`,
  the `doxygen-awesome-*.js` and `doxygen-custom/*` files;
  possibly adjust `.npmignore`
- `npm version 7.0.0`
- push the `xpack-develop` branch to GitHub
- the `postversion` npm script should also update tags via `git push origin --tags`

### Update the repo

When the package is considered stable:

- with a Git client (VS Code is fine)
- merge `xpack-develop` into `xpack`
- push to GitHub
- select `xpack-develop`

## Share on Twitter

- in a separate browser windows, open [TweetDeck](https://tweetdeck.twitter.com/)
- using the `@xpack_project` account
- paste the release name like **µOS++ IIIe v7.0.0 released**
- paste the link to the Web page
  [release](https://micro-os-plus.github.io/micro-os-plus/iii/releases/)
- click the **Tweet** button
