// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedServers/Public/Game/DS_GameMode.h"

DEFINE_LOG_CATEGORY(LogDS_MMOGameMode);

 ADS_GameMode::ADS_GameMode()
{
}

void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	#if WITH_GAMELIFT
		InitGameLift();
	#endif
}

void ADS_GameMode::SetServerParameters(FServerParameters& ServerParameters)
{
    //AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
    if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), ServerParameters.m_authToken))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("AUTH_TOKEN: %s"), *ServerParameters.m_authToken)
    }

    if (FParse::Value(FCommandLine::Get(), TEXT("-awsregion="), ServerParameters.m_awsRegion))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("AWS_REGION: %s"), *ServerParameters.m_awsRegion)
    }


    if (FParse::Value(FCommandLine::Get(), TEXT("-accesskey="), ServerParameters.m_accessKey))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("ACCESS_KEY: %s"), *ServerParameters.m_accessKey)
    }
    if (FParse::Value(FCommandLine::Get(), TEXT("-secretkey="), ServerParameters.m_secretKey))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("SECRET_KEY: % s"), *ServerParameters.m_secretKey)
    }
    if (FParse::Value(FCommandLine::Get(), TEXT("-sessiontoken="), ServerParameters.m_sessionToken))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("SESSION_TOKEN: %s"), *ServerParameters.m_sessionToken)
    }

    //The Host/compute-name of the GameLift Anywhere instance.
    if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), ServerParameters.m_hostId))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("HOST_ID: %s"), *ServerParameters.m_hostId)
    }

    //The Anywhere Fleet ID.
    if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), ServerParameters.m_fleetId))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("FLEET_ID: %s"), *ServerParameters.m_fleetId)
    }

    //The WebSocket URL (GameLiftServiceSdkEndpoint).
    if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), ServerParameters.m_webSocketUrl))
    {
        UE_LOG(LogDS_MMOGameMode, Log, TEXT("WEBSOCKET_URL: %s"), *ServerParameters.m_webSocketUrl)
    }



    FString glProcessId = "";
    if (FParse::Value(FCommandLine::Get(), TEXT("-processId="), glProcessId))
    {
        ServerParameters.m_processId = TCHAR_TO_UTF8(*glProcessId);
    }
    else
    {
        // If no ProcessId is passed as a command line argument, generate a randomized unique string.
        FString TimeString = FString::FromInt(std::time(nullptr));
        FString ProcessId = "ProcessId_" + TimeString;
        ServerParameters.m_processId = TCHAR_TO_UTF8(*ProcessId);
    }

    //The PID of the running process
    UE_LOG(LogDS_MMOGameMode, Log, TEXT("PID: %s"), *ServerParameters.m_processId);
}

void ADS_GameMode::ParseCommandLinePort(int32& OutPort)
{
    TArray<FString> CommandLineTokens;
    TArray<FString> CommandLineSwitches;
    FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
    for (const FString& Switch : CommandLineSwitches)
    {
        FString Key;
        FString Value;
        if (Switch.Split("=", &Key, &Value))
        {
            if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase))
            {
                OutPort = FCString::Atoi(*Value);
                return;
            }
        }
    }
}

void ADS_GameMode::InitGameLift()
 {
     UE_LOG(LogDS_MMOGameMode, Log, TEXT("Initializing the GameLift Server"));

     FGameLiftServerSDKModule* GameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

     //Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
     FServerParameters ServerParameters;

     SetServerParameters(ServerParameters);

     //InitSDK establishes a local connection with GameLift's agent to enable further communication.
     //Use InitSDK(ServerParameters) for a GameLift Anywhere fleet. 
     //Use InitSDK() for a GameLift managed EC2 fleet.
     GameLiftSdkModule->InitSDK(ServerParameters);
    
     auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession GameSessionIn)
     {
         FString GameSessionId = FString(GameSessionIn.GetGameSessionId());
         UE_LOG(LogDS_MMOGameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
         GameLiftSdkModule->ActivateGameSession();
     };

     M_Parameters.OnStartGameSession.BindLambda(onGameSession);
    
     auto onProcessTerminate = [=]()
     {
         UE_LOG(LogDS_MMOGameMode, Log, TEXT("Game Server Process is terminating"));
         GameLiftSdkModule->ProcessEnding();
     };

     M_Parameters.OnTerminate.BindLambda(onProcessTerminate);

     auto onHealthCheck = []()
     {
         UE_LOG(LogDS_MMOGameMode, Log, TEXT("Performing Health Check"));
         return true;
     };

     M_Parameters.OnHealthCheck.BindLambda(onHealthCheck);

     int32 Port = FURL::UrlConfig.DefaultPort;
     ParseCommandLinePort(Port);

     M_Parameters.port = Port;
     TArray<FString> logfiles;
     logfiles.Add(TEXT("Runeterra/Saved/Logs/Runeterra.log"));
     M_Parameters.logParameters = logfiles;


     UE_LOG(LogDS_MMOGameMode, Log, TEXT("Calling Process Ready"));
     GameLiftSdkModule->ProcessReady(M_Parameters);
 }
