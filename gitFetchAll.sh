#!/bin/bash
for branch in `git branch -a | grep remotes | grep -v HEAD | grep -v master`; do
    git checkout ${branch##*/} $branch
#    git branch --track ${branch##*/} $branch
done
