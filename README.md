#  Universal Server

## Branch description

- main branch is a release branch.
- dev branch is a develop branch. 
- feature branch is a develop new feature branch.

when develop a new feature, You must git close from dev branch. Then you can commit your code to feature branch.

admin user will merge code to develop branch after review your code.

The main branch will only be used for release.

## Directory description

- dl directory will keep some related third-party libraries.

## Git commit format

git commit format must comply with the [conventional commits](https://www.conventionalcommits.org/zh-hans/v1.0.0/).

If you do not want to trace `.idea`, you can `git rm -r --cached .idea`, then add `.idea` in your .gitignore file.

## How to manage branches

1. Switch local branch

Usage scenario: switching when there are branches A and B locally. For example, the current working branch is A and needs to be switched to B

```shell
git checkout B
```

2. Switch remote branch

Usage scenario: There is only branch A locally, and there are A and B remotely. You need to switch the local branch from A to branch B

- First check all branches (including remote ones)

```shell
git branch -a
```

- Switch branch

```shell
git checkout -b B origin/B 
// Checkout the remote B branch, name it locally as B branch, and switch to the local B branch
```

3. Local branch associated with remote branch

```shell
git branch --set-upstream {local branch} origin/{remote branch}
```

4. View local and remote branch relationships

```shell
git config --list
```

### Branch usage process

+-----------+ \
\|&ensp;&ensp;&ensp;main &ensp; &ensp;\| \
+-----------+ \
&ensp;&ensp;&ensp;&ensp;&ensp;⬆ \
+-----------+ \
\|&ensp;&ensp;&ensp;&ensp;dev &ensp; &ensp;\| \
+-----------+ \
&ensp;&ensp;&ensp;&ensp;&ensp;⬆ \
+-----------+ \
\|&ensp;&ensp;branch &ensp;\| \
+-----------+ 

## build

```shell
chmod +x 
./build.sh
```