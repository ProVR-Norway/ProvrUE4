%%%%%%%%%%%%%%%%%%% AFTER MPACKEGING OF UE4 DEDICATED SERVER LINUX %%%%%%%%%%%%%%%%%%%%%%%%%

Build the dedicated server, and place the docker file and the setup.bat file in the same directory as the LinuxServer folder created
during the packaging. The follow the guide bellow:


Set up a single dedicated server on GCE or with multiple servers through Linux VM on GCE.

If you want many servers on the same VM instance with different port,
change the port on the EXPOSE field in the docker file. THEN enter the Setup.bat file 
and change it to include the port that is going to be active on that specific image, read comments in the bat file.
If you want one single dedicated server on GCE, continue to Run Setup.bat


Run Setup.bat
make sure that docker is installed and is running on your local computer




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    SINGLE SERVER GCE     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

If you are going to run the container on a chrominum container, do the following:

Copy following into GCE terminal:
gcloud compute instances update-container provr-server          --container-image=gcr.io/vr-collaboration-room/provr-server:latest

when GCE asks:
Did you mean zone [europe-west4-a] for instance: [provr-server] (Y/n)?

press n


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    MULTIPLE SERVERS GCE VM WITH UBUNTU     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

If you are going to run the server on a VM with ubuntu,
go to your VM instanse, press shh, and write the following:

sudo gcloud docker -- pull gcr.io/vr-collaboration-room/provr-server:latest(set in the name of your taged image here, if you changed port)

sudo docker image ls
 
sudo docker run -d -it -p 7777:7777/udp dd4a24724093 (place your conainer id here)


want to monitor your server?
sudo docker attach --sig-proxy=false Container_ID  	-- See live log on one container
sudo docker logs Container_ID    					-- See the log on one container

sudo docker image ls 								-- See image overview
sudo docker ps 										-- See all active containers
sudo docker image rm Image_ID						-- Remove image
sudo docker rm Container_ID							-- Remove Container
udo docker kill container Container_ID				-- Kill container
sudo docker run -d -it -p portOnUbuntu(ex. 7779):7777/udp Image_ID  -- Set container port and run contianer

docker kill $(docker ps -q)							-- Stop all containers
docker rm $(docker ps -a -q)						-- Delete all containers
docker rmi $(docker images -q)						-- Delete all images


ctrl P + ctrl Q 									-- Stop getting container log, and enter VM ssh
