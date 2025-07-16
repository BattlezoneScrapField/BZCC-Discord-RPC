#include "DiscordManager.h"

#include <ScriptUtils.h>

#include <chrono>
#include <format>

DiscordManager::DiscordManager(const char* appID)
	: appID(appID), sendPresence(true), startTime(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count())
{
	DiscordEventHandlers handlers{};

	handlers.ready = HandleReady;
	handlers.disconnected = HandleDisconnected;
	handlers.errored = HandleErrored;
	handlers.joinGame = HandleJoin;
	handlers.spectateGame = HandleSpectate;
	handlers.joinRequest = HandleJoinRequest;

	Discord_Initialize(appID, &handlers, 1, nullptr);
}

DiscordManager::~DiscordManager()
{
	Discord_Shutdown();
}

void DiscordManager::Update()
{
	if (sendPresence)
	{
		DiscordRichPresence discordPresence{};

		std::string details = std::format("Frustration level {}", 10); // probably make this a member variable or something -VT

		discordPresence.state = "bzcc something";
		discordPresence.details = details.c_str();
		discordPresence.startTimestamp = startTime;
		discordPresence.endTimestamp = 0; // not sure what the correct value is play around with it -VT
		discordPresence.largeImageKey = "your image";
		discordPresence.smallImageKey = "feuker";
		discordPresence.partyId = "Vets";
		discordPresence.partySize = 1;
		discordPresence.partyMax = 6;
		discordPresence.partyPrivacy = DISCORD_PARTY_PUBLIC;
		discordPresence.matchSecret = "abcxyz";
		discordPresence.joinSecret = "join";
		discordPresence.spectateSecret = "look";
		discordPresence.instance = 0;

		Discord_UpdatePresence(&discordPresence);
	}
	else
	{
		Discord_ClearPresence();
	}
}

void DiscordManager::HandleReady(const DiscordUser* request)
{
	PrintConsoleMessage(std::format("Discord connected to user {}{} - {}", request->username, request->discriminator, request->userId).c_str());
}

void DiscordManager::HandleDisconnected(int errorCode, const char* message)
{
	PrintConsoleMessage("Disconnected");
}

void DiscordManager::HandleErrored(int errorCode, const char* message)
{
	PrintConsoleMessage("Error");
}

void DiscordManager::HandleJoin(const char* joinSecret)
{
	PrintConsoleMessage("Join");
}

void DiscordManager::HandleSpectate(const char* spectateSecret)
{
	PrintConsoleMessage("Spectate");
}

void DiscordManager::HandleJoinRequest(const DiscordUser* request)
{
	PrintConsoleMessage("Someone requested to join...");
}
