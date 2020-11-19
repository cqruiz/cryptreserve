echo "Gen2 License client Install"
/usr/z1090/bin/gen2_init
sleep 5s
echo "Gen2 License client installed"
sleep 5s
echo "Configure Gen2 License client"
/usr/z1090/bin/clientconfig_cli -g2s1 $LM
echo "Gen2 License client configured"
sleep 5s
echo "Reset UIM"
/usr/z1090/bin/uimreset -r
echo "UIM reset"
sleep 5s
echo "Setup iptables"
/home/ibmsys1/volumes/iptables-script.sh
echo “iptables set”
/bin/bash
