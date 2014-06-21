# Assumptions:
# Beacon every minute
# Log generated with picocom /dev/ttyUSB0 | ts > /tmp/eqail.log

START=`head /tmp/equail.log | grep -a 'sleeping' | cut -f 1-3 -d ' '  | head -n 1`
START_S=`date -d "$START" +%s`
END=`date +%s`

SECONDS=$(( ($END - $START_S) / 60 ))

NUMBER=`grep -a '\*\*\*\*\*' /tmp/equail.log  | wc -l`

echo "scale=10 
$NUMBER / $SECONDS" | bc

