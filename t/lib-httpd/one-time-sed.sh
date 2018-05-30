#!/bin/sh

if [ -e one-time-sed ]; then
	"$GIT_EXEC_PATH/git-http-backend" >out

	sed "$(cat one-time-sed)" <out >out_modified

	if diff out out_modified >/dev/null; then
		cat out
	else
		cat out_modified
		rm one-time-sed
	fi
else
	"$GIT_EXEC_PATH/git-http-backend"
fi
