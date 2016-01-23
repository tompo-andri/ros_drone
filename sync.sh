rsync -e "ssh -i $PIKEYPATH" \
--verbose --links --delete --compress --recursive --times --human-readable --no-perms \
--exclude=".git" \
--exclude=".gitignore" \
--exclude="sync.sh" \
--exclude="catkin_workspace/build" \
--exclude="devel" \
--include="devel/setup.bash" \
--include="devel/setup.sh" \
--include="devel/setup.zsh" \
catkin_workspace \
pi@10.5.5.1:/home/pi/ros_drone