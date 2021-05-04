// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Network/ProVRSessionInterface.h"
#include "CoreMinimal.h"
#include "ProVRGameInstance.h"
#include "Kismet/GameplayStatics.h"




void UProVRSessionInterface::CreateSession(FString SessionName, FString MapName, int32 MaxPlayers)
{
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetVerb("POST");
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
	RequestJson->SetStringField("sessionName", SessionName);
	RequestJson->SetNumberField("maxParticipants", MaxPlayers);  //possible cast issues
	RequestJson->SetStringField("hostUsername", "Username");
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);
	TArray<uint8>* RequestContent = nullptr;
	const FTCHARToUTF8 Converter(*OutputString, OutputString.Len());
	RequestContent->Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
	//Request->SetContent(RequestContent);


	Request->SetURL("https://api-gateway-iu3tuzfidq-ez.a.run.app");
	Request->OnProcessRequestComplete().BindUObject(this, &UProVRSessionInterface::OnCreateSessionComplete);
	Request->ProcessRequest();
}

void UProVRSessionInterface::OnCreateSessionComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!Response.IsValid())
	{
		FString WarningMessage = TEXT("Somewent wrong!");
		UE_LOG(LogTemp, Warning, TEXT("Error Creating Session: UProVRSessionInterface::OnCreateSessionComplete"));
		OnCreateSessionCompleteDelegate.Broadcast(false, WarningMessage);
	}
	if (Response->GetResponseCode() == 200)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		FString JsonField = JsonObject->GetStringField(TEXT("message"));
		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			if (!JsonField.IsEmpty())
			{
				OnCreateSessionCompleteDelegate.Broadcast(true, JsonField);
			}
			
		}

	}
}


void UProVRSessionInterface::SearchSessions()
{
	FHttpModule* Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

	Request->SetHeader(TEXT("Content-Type"), TEXT("Something"));  //Something usefull
	Request->SetVerb("GET");
	Request->SetURL("https://api-gateway-iu3tuzfidq-ez.a.run.app");
	Request->OnProcessRequestComplete().BindUObject(this, &UProVRSessionInterface::OnSearchSessionComplete);
	Request->ProcessRequest();

}


void UProVRSessionInterface::OnSearchSessionComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!Response.IsValid())
	{
		FString WarningMessage = TEXT("Somewent wrong!");
		UE_LOG(LogTemp, Warning, TEXT("Error Searching Session: UProVRSessionInterface::OnSearchSessionComplete"));
		OnSearchSessionCompleteDelegate.Broadcast(true);
	}
	if (Response->GetResponseCode() == 200)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		//DisplayedSessions.Add(FString, FString);
	}
}


void UProVRSessionInterface::JoinSession(FString SessionName)
{
	if (UProVRGameInstance* GameInstance = UProVRGameInstance::GetCurrentGameInstance())
	{
		if (UWorld* World = GameInstance->GetWorld())
		{
			FString* URLAddress_ = DisplayedSessions.Find(SessionName);
			UGameplayStatics::OpenLevel(World, FName(*URLAddress_), false, "");
		}
	}
}
