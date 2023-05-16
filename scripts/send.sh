interface=${1:-'enp10s0f0'}
app='../isochron send'
CPU=5
src_ip='192.168.25.11'
args='--step_threshold 0.00002'

ptp4l -i $interface -2 -P --step_threshold 0.00002 &
phc2sys -a -rr &

tc qdisc add dev $interface root taprio num_tc 5 \
	map 0 1 2 3 4 \
	queues 1@0 1@1 1@2 1@3 1@4 \
	base-time 0 \
    clockid CLOCK_TAI \
	sched-entry S 10  50000 \
	sched-entry S 0f 450000 \

taskset -c 5 $app \
	--interface $interface \
	--cycle-time 0.0005 \
	--frame-size 64 \
	--num-frames 1000000 \
	--client $src_ip \
	--quiet \
	--sync-threshold 2000 \
	--output-file isochron.dat \
	--taprio \
	--priority 4 \
	--sched-rr \
	--sched-priority 98 \
	--window-size 50000