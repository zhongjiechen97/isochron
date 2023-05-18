interface=${1:-'enp10s0f0'}
app='../isochron rcv'
CPU=5
args='--step_threshold 0.00002'

sudo ptp4l -i $interface -2 -P --step_threshold 0.00002 &
# sudo phc2sys -a -rr &
sudo phc2sys -s CLOCK_REALTIME -c $interface -m -O 0 &
taskset -c 5 $app \
	--interface $interface \
	--quiet \
	--sched-rr \
	--sched-priority 98