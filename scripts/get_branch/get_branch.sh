#!/bin/bash

# Get the name of the working branch a write it to a file if correct
branch=$(git branch 2>&1)

if [[ $branch != fatal* ]] && [[ $branch != '* (HEAD'* ]]
then
	echo "$branch" | $(sed -n -e 's/^\* \(.*\)/\1/p' > $WEBOTS_HOME/resources/branch.txt)
fi

