# ProvrUE4

[![GitHub Super-Linter](https://github.com/ProVR-Norway/ProvrUE4/workflows/Lint%20Code%20Base/badge.svg)](https://github.com/marketplace/actions/super-linter) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![GitHub issues](https://img.shields.io/github/issues/ProVR-Norway/ProvrUE4.svg)](https://GitHub.com/Naereen/StrapDown.js/issues/)
[![GitHub stars](https://img.shields.io/github/stars/ProVR-Norway/ProvrUE4.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/Naereen/StrapDown.js/stargazers/) 

VR Collaboration Room. Unreal Engine 4 plug-ins and project files.

### Dedicated server deployment

The Google Cloud Platform (GCP) service Compute Engine was used to deploy the dedicated servers so that online sessions can be created. All the servers run on seperate Docker containers on different network ports on a Ubuntu VM instance. To allow all UDP traffic on all the ports of the VM instance, we needed to add a new firewall rule in GCP with the compute engine servicde account as the target. Only after doing this we were able to connect a network client to the server.
