#!/bin/zsh


use_colors="1";

if [ $use_colors -eq "1" ]; then
	fg_black=$'\e[0;30m';
	fg_green=$'\e[0;32m';
	fg_yellow=$'\e[0;33m';
	fg_red=$'\e[0;31m';
	fg_blue=$'\e[0;34m';
	fg_magenta='\e[0;35m';
	fg_cyan=$'\e[0;36m';
	fg_reset=$'\e[;0m';
else
	fg_black=$'';
	fg_green=$'';
	fg_yellow=$'';
	fg_red=$'';
	fg_blue=$'';
	fg_magenta='';
	fg_cyan=$'';
	fg_reset=$'';
fi

echo "Test suite"
echo "==========="

wd=`pwd`;
# export PATH="${wd}/..:${PATH}";
# export LD_LIBRARY_PATH="${wd}/..:${LD_LIBRARY_PATH}";

# echo $LD_LIBRARY_PATH
# How many tests
NUM=`ls test*.py | sort | sed -e 's/test//' | sed -e 's/.py$//' | wc -l`
NUM1=1

echo "$NUM tests exist"
## ALL=`seq -s ' ' $NUM`;

[ -n "$1" ] && NUM1="$@";
[ -n "$1" ] && NUM="$@";
[ -n "$2" ] && NUM="$2";
[ -n "$2" ] && NUM1="$1";

for i in `seq $NUM1 $NUM`; do
  echo "now: -> $i <-"
  rm -f /tmp/me;
  echo "Now performing: test${i}.py"
  ./test${i}.py > /tmp/me;
  OUT=`diff /tmp/me output${i}.txt`;
  [ -z "$OUT" ] &&
  {
    echo "${fg_green}No difference! -> Test OK${fg_reset}";
  } || {
	  echo "${fg_red}Difference!! Check yourself:${fg_reset}";
	  echo "diff /tmp/me output${i}.txt: ";
	  diff /tmp/me output${i}.txt
  };
done
