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


## git commit format

git commit format must comply with the [conventional commits](https://www.conventionalcommits.org/zh-hans/v1.0.0/).

If you do not want to trace `.idea`, you can `git rm -r --cached .idea`, then add `.idea` in your .gitignore file.