// Copyright 2021, MIT License, University of South-Eastern Norway - Kongsberg Digital


#include "Network/ProVRSessionInterface.h"
#include "CoreMinimal.h"




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
	TArray<uint8>* RequestContent;
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
		UE_LOG(LogTemp, Warning, TEXT("Error Creating Python Session! No Response from Master Server"));
		OnCreateSessionCompleteDelegate.Broadcast(true, WarningMessage);
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

	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetVerb("POST");
	TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject);
	RequestJson->SetStringField("sessionName", SessionName);
	RequestJson->SetNumberField("maxParticipants", MaxPlayers);  //possible cast issues
	RequestJson->SetStringField("hostUsername", "Username");
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);
	TArray<uint8>* RequestContent;
	const FTCHARToUTF8 Converter(*OutputString, OutputString.Len());
	RequestContent->Append(reinterpret_cast<const uint8*>(Converter.Get()), Converter.Length());
	//Request->SetContent(RequestContent);


	Request->SetURL("https://api-gateway-iu3tuzfidq-ez.a.run.app");
	Request->OnProcessRequestComplete().BindUObject(this, &UProVRSessionInterface::OnCreateSessionComplete);
	Request->ProcessRequest();

}

void UProVRSessionInterface::JoinSession(FString SessionName)
{

}
