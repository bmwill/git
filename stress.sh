#!/bin/sh

test=$1; shift
test=$(cd git/t && echo $test*.sh)
: ${GIT_STRESS_LOAD:=$(( 2 * $(grep -c ^processor /proc/cpuinfo)))}
: ${GIT_STRESS_ROOT:=/tmp/git-stress}

#export GIT_TEST_INSTALLED=/tmp/git-grte/bin
#export GIT_TEST_EXEC_PATH=/tmp/git-grte/lib/git-core

mkdir -p "$GIT_STRESS_ROOT" || exit 1
fail="$GIT_STRESS_ROOT/fail"
rm -f "$fail"
trap 'echo aborted >"$fail"' TERM INT HUP
for i in $(seq $GIT_STRESS_LOAD); do
	(
		# It doesn't really matter where this is as long as we're not
		# likely to hit any other listening services, nor conflict with
		# dynamic ports.
		PORT=$((8000 + i))
		export LIB_GIT_DAEMON_PORT=$PORT
		export LIB_HTTPD_PORT=$PORT
		export JGIT_DAEMON_PORT=$PORT

		root=$GIT_STRESS_ROOT/root-$i &&
		mkdir -p "$root" &&
		cd git/t &&
		while ! test -e "$GIT_STRESS_ROOT/fail"
		do
			if ./$test --root="$root" -v -i >"$root/output" 2>&1
			then
				echo >&2 "OK $i"
			else
				echo >&2 "FAIL $i"
				ln -sfT "root-$i" "$fail"
			fi
		done
	) &
done
wait
test -d "$fail" && cat "$fail/output"
