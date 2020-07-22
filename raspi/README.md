# raspi zero wh2台
ホスト名は
サーバー: raspberrypi0, PCと接続
クライアント: raspberrypi1, マウスと接続


# ホスト側作業
```bash
git clone https://github.com/future731/mouse-linux
```
2枚のSDカードにOSをインストール
~/.ssh/configにssh_configの中身を登録
それぞれraspiwh0，raspiwh1として登録してある
/mediaにマウントされたやつの/boot/に
```bash
sudo touch /boot/ssh
sudo vi /rootfs/etc/wpa_supplicant/wpa_suplicant.conf # wifi設定
```
N=0,1として
```bash
sudo vi /etc/hosts
sudo vi /etc/hostname
```
してraspberrypiNとする
起動して，
```bash
ssh raspiwhN
passwd # 初期パスはraspberryなのでpasswdする
```

# サーバー，クライアント共通設定
```bash
git clone https://github.com/future731/mouse-linux
cd mouse-linux/raspi
mkdir build
cd build
cmake ..
make
sudo /home/pi/mouse-linux/raspi/setup_ssh.sh
sudo /home/pi/mouse-linux/raspi/setup_deps.sh
```

# サーバー設定
raspberrypi0の方
サーバー側
```bash
sudo /home/pi/mouse-linux/raspi/setup_otg.sh
sudo /home/pi/mouse-linux/raspi/rc.local /etc/rc.local
```

# クライアント設定
```bash
# 99-mouse.rulesをマウスに合わせて編集
sudo cp /home/pi/mouse-linux/raspi/99-mouse.rules /etc/udev/rules.d
sudo udevadm control --reload
```

# サーバー側起動
sudo /home/pi/mouse-linux/raspi/server
# クライアント側起動
sudo /home/pi/mouse-linux/raspi/client
