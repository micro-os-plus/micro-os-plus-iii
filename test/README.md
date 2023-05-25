# README

## Download repos to development area

```bash
mkdir micro-os-plus-iii && cd micro-os-plus-iii

git clone https://github.com/xpacks/scripts.git scripts.git

git clone https://github.com/micro-os-plus/micro-os-plus-iii.git micro-os-plus-iii.git
git clone https://github.com/micro-os-plus/micro-os-plus-iii-cortexm.git micro-os-plus-iii-cortexm.git
git clone https://github.com/xpacks/arm-cmsis-rtos-validator.git arm-cmsis-rtos-validator.git
git clone https://github.com/micro-os-plus/posix-arch.git posix-arch.git
git clone https://github.com/xpacks/chan-fatfs.git chan-fatfs.git

git clone https://github.com/micro-os-plus/eclipse-test-projects.git eclipse-test-projects.git

# git clone https://github.com/micro-os-plus/eclipse-demo-projects.git eclipse-demo-projects.git

# git clone https://github.com/micro-os-plus/helper-scripts.git helper-scripts.git
```

## Make links from central store to development repos

```bash
from="/Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii"

mkdir -pv "${HOME}/Library/xPacks/ilg" && cd "${HOME}/Library/xPacks/ilg"
for repo in scripts.git micro-os-plus-iii.git micro-os-plus-iii-cortexm.git arm-cmsis-rtos-validator.git posix-arch.git chan-fatfs.git
do
  rm -rf "${repo}"
  ln -s "${from}/${repo}" "${repo}"
done
```

## Informative

- <https://github.com/xpacks/scripts/raw/master/xpacks-paths.sh>

```bash
curl -L https://github.com/xpacks/scripts/raw/master/xpacks-paths.sh -o "${HOME}/Downloads/xpacks-paths.sh"
```

- <https://github.com/xpacks/scripts>
- <https://github.com/xpacks/scripts/raw/master/bootstrap.sh>

```bash
curl -L https://github.com/xpacks/scripts/raw/master/bootstrap.sh -o "${HOME}/Downloads/bootstrap.sh"
```

## Eclipse

Download Eclipse Embedded CDT

- <https://www.eclipse.org/downloads/packages/>

```bash
xattr -dr com.apple.quarantine Eclipse.app
mv Eclipse.app Eclipse-ecdt-2023-03.app
```

Open Eclipse and create `micro-os-plus-iii-workspace`

Install new software from 2023-03

- C/C++ LLVM-Family Compiler Build Support
- Eclipse XML Editors and Tools

Update workspace preferences:

- <https://eclipse-embed-cdt.github.io/eclipse/workspace/preferences/>

## eclipse-test-projects

### synthetic-posix-tests-micro-os-plus

- run `scripts/generate.sh`
- import `synthetic-posix-tests-micro-os-plus` into Eclipse, without copy

#### test-cmsis-rtos-valid

- build `test-cmsis-rtos-valid-clang-debug`
- run `osx-test-cmsis-rtos-valid-clang.debug.launch`

- build `test-cmsis-rtos-valid-clang-release`
- run `osx-test-cmsis-rtos-valid-clang.release.launch`

#### test-mutex-stress-clang

- build `test-mutex-stress-clang-debug`
- run `osx-mutex-stress-clang.debug.launch`

- build `test-mutex-stress-clang-release`
- run `osx-mutex-stress-clang.release.launch`

#### test-rtos-clang

- build `test-rtos-clang-debug`
- run `osx-rtos-clang.debug.launch`

- build `test-rtos-clang-release`
- run `osx-rtos-clang.release.launch`

### f4discovery-tests-micro-os-plus

- run `scripts/generate.sh`
- import `f4discovery-tests-micro-os-plus` into Eclipse, without copy
