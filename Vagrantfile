###################################################################
#File Name	    : Vagrantfile                                                                                              
#Description	  : This is a Vagrantfile for an automated
#                 deployment of a InfluxDB.                                                                                 
#Args           :                                                                                           
#Author       	: Yves Wetter                                             
#Email         	: yves.wetter@edu.tbz.ch                                         
###################################################################
Vagrant.configure("2") do |config|
  config.vm.define "m242-influxdb" do |influxdb|
    influxdb.vm.box = "ubuntu/bionic64"
    influxdb.vm.hostname = "m242-influxdb"
    influxdb.vm.network "public_network" , bridge: [
      "Intel(R) Wi-Fi 6 AX201 160MHz",
      "WLAN",
      "Drahtlos-LAN-Adapter WLAN",
      "en0: Wi-Fi (AirPort)"
    ]
    influxdb.vm.synced_folder "install/", "/mnt"
    influxdb.vm.provision :shell, :path => "./install/install.sh"
  end
end