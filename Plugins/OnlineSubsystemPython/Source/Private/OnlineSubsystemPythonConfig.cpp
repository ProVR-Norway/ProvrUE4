
#include "OnlineSubsystemPythonConfig.h"

UOnlineSubsystemPythonConfig::UOnlineSubsystemPythonConfig()
	: ServerAddress("34.90.182.107:8081")
	, AuthorizationTicket("")
{

}

void UOnlineSubsystemPythonConfig::SetServerAddress(FString NewServerAddress)
{
	ServerAddress = NewServerAddress;
	SaveConfig();
}

void UOnlineSubsystemPythonConfig::SetAuthorizationTicket(FString NewAuthorizationTicket)
{
	AuthorizationTicket = NewAuthorizationTicket;
	SaveConfig();
}