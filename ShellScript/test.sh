drop2() {
  echo ${@:3}
}

drop2 1 2 3 4 5 6
# -> "3 4 5 6"と表示される

echo ${@:0}