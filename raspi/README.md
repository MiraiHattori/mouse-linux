# raspi zero wh2台

ホスト名は
サーバー: raspberrypi0, PCと接続
クライアント: raspberrypi1, マウスと接続

sshのconfigに，それぞれraspiwh0，raspiwh1として登録してある前提

# 共通設定
SDカードにOSをインストール

/mediaにマウントされたやつの/boot/に
sudo touch /boot/ssh
sudo vi /rootfs/etc/wpa_supplicant/wpa_suplicant.confしてwifi設定
sudo vi /etc/hostsしてraspberrypiNとする
sudo vi /etc/hostnameしてraspberrypiNとする
起動して，ssh pi@raspberrypiN．初期パスはraspberryなのでpasswdする
setup_ssh.shをscpしておいて
sudo ./setup_ssh.sh

# サーバー設定
raspberrypi0の方
サーバー側
setup_otg.shとhid.shをscpしておいて
sudo ./setup_otg.sh
/etc/rc.localのexit 0の前に/home/pi/hid.shを書いておく

# クライアント設定
99-mouse.rulesを/etc/udev/rules.dにおく
sudo udevadm control --reload

# 共通設定
mouse-linux/raspiを両方でclone? scp?

# サーバー側起動
mouse-linux/raspi/build/serverをsudoで起動
# クライアント側起動
mouse-linux/raspi/build/clientをsudoで起動
