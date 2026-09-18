/**
 * =============================================================================
 * SourceMod Discord Extension
 * Copyright 2024-2026 ProjectSky
 * =============================================================================
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "natives/entity_natives_common.h"
#include "entities/discord_message.h"
#include "entities/discord_attachment.h"
#include "entities/discord_reaction.h"
#include "utils/discord_embed.h"
#include "utils/discord_component.h"
#include "utils/discord_poll.h"

#include <fstream>

static cell_t message_CreateFromId(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* messageId;
	pContext->LocalToString(params[2], &messageId);

	char* channelId;
	pContext->LocalToString(params[3], &channelId);

	dpp::snowflake messageFlake, channelFlake;
	if (!ParseSnowflake(pContext, messageId, messageFlake)) return 0;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	dpp::message message_obj;
	message_obj.id = messageFlake;
	message_obj.channel_id = channelFlake;

	DiscordMessage* pDiscordMessage = new DiscordMessage(message_obj, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateEmpty(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordMessage* pDiscordMessage = new DiscordMessage(discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordMessage* pDiscordMessage = new DiscordMessage(content, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithChannel(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	char* content;
	pContext->LocalToString(params[3], &content);

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	DiscordMessage* pDiscordMessage = new DiscordMessage(channelFlake, content, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[2]);
	if (!embed) return 0;

	DiscordMessage* pDiscordMessage = new DiscordMessage(embed, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_CreateWithChannelEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	DiscordMessage* pDiscordMessage = new DiscordMessage(channelFlake, embed, discord);
	Handle_t handle = Handles.Create(pContext, pDiscordMessage, HandleId::DiscordMessage);
	if (!handle) return 0;
	return handle;
}

static cell_t message_FetchMessage(IPluginContext* pContext, const cell_t* params)
{
	DiscordClient* discord = Handles.GetPointer<DiscordClient>(pContext, params[1]);
	if (!discord) return 0;

	char* messageId;
	pContext->LocalToString(params[2], &messageId);

	char* channelId;
	pContext->LocalToString(params[3], &channelId);

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[5];

	dpp::snowflake messageFlake, channelFlake;
	if (!ParseSnowflake(pContext, messageId, messageFlake)) return 0;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;

	Handle_t client_handle = discord->GetHandle();
	discord->Messages().Get(messageFlake, channelFlake, [callback = AsyncCallback(client_handle, callback, data)](const dpp::confirmation_callback_t& confirmation) {
		callback.Result<DiscordMessage>(confirmation);
	});

	return 1;
}

static cell_t message_GetContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetContent());
	return 1;
}

static cell_t message_GetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetChannelId().c_str());
	return 1;
}

static cell_t message_GetAuthor(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	Handle_t handle = message->GetAuthorHandle();

	if (!handle) {
		pContext->ReportError("Could not create author handle");
		return 0;
	}

	return handle;
}

static cell_t message_GetAuthorNickName(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetAuthorNickName().c_str());
	return 1;
}

static cell_t message_GetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return static_cast<cell_t>(message->GetType());
}

static cell_t message_GetMessageId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	pContext->StringToLocal(params[2], params[3], message->GetMessageId().c_str());
	return 1;
}

static cell_t message_Edit(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	message->Edit(content, callback, data);
	return 1;
}

static cell_t message_EditEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	message->EditEmbed(content, embed, callback, data);
	return 1;
}

static cell_t message_EditFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->EditFromObject(callback, data);
	return 1;
}

static cell_t message_Delete(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Delete(callback, data);
	return 1;
}

static cell_t message_Pin(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Pin(callback, data);
	return 1;
}

static cell_t message_Unpin(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Unpin(callback, data);
	return 1;
}

static cell_t message_AddReaction(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* emoji;
	pContext->LocalToString(params[2], &emoji);
	message->AddReaction(emoji);
	return 1;
}

static cell_t message_RemoveReaction(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* emoji;
	pContext->LocalToString(params[2], &emoji);
	message->RemoveReaction(emoji);
	return 1;
}

static cell_t message_RemoveAllReactions(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->RemoveAllReactions(); return 1;
}

static cell_t message_Reply(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	message->Reply(content, callback, data);
	return 1;
}

static cell_t message_ReplyEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[3]);
	if (!embed) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	message->ReplyEmbed(content, embed, callback, data);
	return 1;
}

static cell_t message_ReplyFromObject(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordMessage* reply_message = Handles.GetPointer<DiscordMessage>(pContext, params[2]);
	if (!reply_message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[3]);

	cell_t data = params[4];
	message->ReplyFromObject(reply_message, callback, data);
	return 1;
}

static cell_t message_Crosspost(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->Crosspost(); return 1;
}

static cell_t message_CreateThread(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* name;
	pContext->LocalToString(params[2], &name);

	uint16_t auto_archive_duration;
	if (!GetNativeAutoArchiveMinutes(pContext, params[3], false, "Auto archive duration", auto_archive_duration)) return 0;
	IPluginFunction* callback = pContext->GetFunctionById(params[4]);

	cell_t data = params[5];
	message->CreateThread(name, auto_archive_duration, callback, data);
	return 1;
}

static cell_t message_GetTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return WriteTimestampString(pContext, params[2], params[3], message->GetTimestamp());
}

static cell_t message_GetEditedTimestamp(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	return WriteTimestampString(pContext, params[2], params[3], message->GetEditedTimestamp());
}

static cell_t message_GetReferencedMessageId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string id = message->GetReferencedMessageId();
	pContext->StringToLocal(params[2], params[3], id.c_str());
	return 1;
}

static cell_t message_GetReferencedChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string id = message->GetReferencedChannelId();
	pContext->StringToLocal(params[2], params[3], id.c_str());
	return 1;
}

static cell_t message_GetReferencedGuildId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string id = message->GetReferencedGuildId();
	pContext->StringToLocal(params[2], params[3], id.c_str());
	return 1;
}

static cell_t message_GetMentionedUserId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string userId = message->GetMentionedUserId(params[2]);
	pContext->StringToLocal(params[3], params[4], userId.c_str());
	return 1;
}

static cell_t message_GetMentionedRoleId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string roleId = message->GetMentionedRoleId(params[2]);
	pContext->StringToLocal(params[3], params[4], roleId.c_str());
	return 1;
}

static cell_t message_GetMentionedChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string channelId = message->GetMentionedChannelId(params[2]);
	pContext->StringToLocal(params[3], params[4], channelId.c_str());
	return 1;
}

static cell_t message_GetMentionedUserIds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	const auto& mentions = message->GetMentionedUsers();
	for (const auto& mention : mentions) {
		array->AddString(mention.first.id.str());
	}

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
}

static cell_t message_GetMentionedRoleIds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	const auto& roles = message->GetMentionedRoles();
	for (const auto& role_id : roles) {
		array->AddString(role_id.str());
	}

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
}

static cell_t message_GetMentionedChannelIds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordHandleArray* array = new DiscordHandleArray(false);
	const auto& channels = message->GetMentionedChannels();
	for (const auto& channel : channels) {
		array->AddString(channel.id.str());
	}

	return Handles.Create(pContext, array, HandleId::DiscordHandleArray);
}

static cell_t message_GetWebhookId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string webhookId = message->GetWebhookId();
	pContext->StringToLocal(params[2], params[3], webhookId.c_str());
	return 1;
}

static cell_t message_GetNonce(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	const char* nonce = message->GetNonce();
	pContext->StringToLocal(params[2], params[3], nonce);
	return 1;
}

static cell_t message_GetURL(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	std::string url = message->GetURL();
	pContext->StringToLocal(params[2], params[3], url.c_str());
	return 1;
}

static cell_t message_SetChannelId(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* channelId;
	pContext->LocalToString(params[2], &channelId);

	dpp::snowflake channelFlake;
	if (!ParseSnowflake(pContext, channelId, channelFlake)) return 0;
	message->SetChannelId(channelFlake);
	return 1;
}

static cell_t message_SetType(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	dpp::message_type type = static_cast<dpp::message_type>(params[2]);
	message->SetType(type);
	return 1;
}

static cell_t message_SetFlags(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	uint16_t flags;
	if (!GetNativeUInt16(pContext, params[2], 0xFFFF, "Message flags", flags)) return 0;

	message->SetFlags(flags);
	return 1;
}

static cell_t message_SetTTS(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->SetTTS(params[2]);
	return 1;
}

static cell_t message_SetNonce(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* nonce;
	pContext->LocalToString(params[2], &nonce);
	message->SetNonce(nonce);
	return 1;
}

static cell_t message_ClearEmbeds(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->ClearEmbeds();
	return 1;
}

static cell_t message_Send(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	IPluginFunction* callback = pContext->GetFunctionById(params[2]);

	cell_t data = params[3];
	message->Send(callback, data);
	return 1;
}

static cell_t message_SetPoll(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordPoll* poll = Handles.GetPointer<DiscordPoll>(pContext, params[2]);
	if (!poll) return 0;

	message->SetPoll(poll);
	return 1;
}

static cell_t message_GetPoll(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	Handle_t handle = message->GetPollHandle();
	if (!handle) return 0;

	return handle;
}

static cell_t message_EndPoll(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->EndPoll();
	return 1;
}

static cell_t message_GetPollAnswerVoters(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	uint32_t answer_id;
	if (!GetNativeUInt32(pContext, params[2], 1000, "Poll answer ID", answer_id)) return 0;
	IPluginFunction* callback = pContext->GetFunctionById(params[3]);
	if (!callback) {
		pContext->ReportError("Invalid callback function");
		return 0;
	}

	cell_t data = params[4];
	message->GetPollAnswerVoters(answer_id, callback, data);
	return 1;
}

static cell_t message_SetContent(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* content;
	pContext->LocalToString(params[2], &content);
	message->SetContent(content);
	return 1;
}

static cell_t message_AddEmbed(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordEmbed* embed = Handles.GetPointer<DiscordEmbed>(pContext, params[2]);
	if (!embed) return 0;

	message->AddEmbed(embed);
	return 1;
}

static bool IsSafeRelativeGamePath(const char* path)
{
	if (!path || !*path) return false;
	if (path[0] == '/' || path[0] == '\\') return false;
	if (path[0] && path[1] == ':') return false;

	const char* part = path;
	while (*part) {
		while (*part == '/' || *part == '\\') part++;
		if (!*part) break;

		const char* end = part;
		while (*end && *end != '/' && *end != '\\') end++;
		if ((end - part) == 2 && part[0] == '.' && part[1] == '.') return false;
		part = end;
	}

	return true;
}

static const char* GetPathBasename(const char* path)
{
	const char* basename = path;
	for (const char* p = path; *p; p++) {
		if (*p == '/' || *p == '\\') basename = p + 1;
	}
	return basename;
}

static cell_t message_AddFile(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	char* path;
	char* filename;
	char* mimetype;
	if (pContext->LocalToString(params[2], &path) != SP_ERROR_NONE ||
		pContext->LocalToString(params[3], &filename) != SP_ERROR_NONE ||
		pContext->LocalToString(params[4], &mimetype) != SP_ERROR_NONE) {
		pContext->ReportError("Invalid string parameter");
		return 0;
	}

	if (!IsSafeRelativeGamePath(path)) {
		pContext->ReportError("DiscordMessage.AddFile path must be relative to the game directory and may not contain '..': %s", path);
		return 0;
	}

	char fullPath[PLATFORM_MAX_PATH];
	g_pSM->BuildPath(Path_Game, fullPath, sizeof(fullPath), "%s", path);

	std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		pContext->ReportError("Could not open Discord upload file: %s", path);
		return 0;
	}

	const std::streamoff size = file.tellg();
	static constexpr std::streamoff kMaxFileBytes = 25 * 1024 * 1024;
	if (size <= 0 || size > kMaxFileBytes) {
		pContext->ReportError("Discord upload file must be between 1 byte and 25 MiB: %s", path);
		return 0;
	}

	std::string content(static_cast<size_t>(size), '\0');
	file.seekg(0, std::ios::beg);
	if (!file.read(content.data(), size)) {
		pContext->ReportError("Could not read Discord upload file: %s", path);
		return 0;
	}

	const char* uploadName = (filename && *filename) ? filename : GetPathBasename(path);
	if (!uploadName || !*uploadName || strchr(uploadName, '/') || strchr(uploadName, '\\')) {
		pContext->ReportError("Invalid Discord upload filename: %s", uploadName ? uploadName : "(null)");
		return 0;
	}

	if (!message->AddFile(uploadName, content, (mimetype && *mimetype) ? mimetype : "application/octet-stream")) {
		pContext->ReportError("Could not attach %s: message would exceed 10 files or 25 MiB total", uploadName);
		return 0;
	}

	return 1;
}

static cell_t message_SetAllowedMentions(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	constexpr cell_t kMaxAllowedMentionIds = 100;
	cell_t users_count = params[4];
	cell_t roles_count = params[6];

	if (users_count < 0 || users_count > kMaxAllowedMentionIds) {
		pContext->ReportError("Invalid allowed mention user count %d (must be 0-%d)", users_count, kMaxAllowedMentionIds);
		return 0;
	}

	if (roles_count < 0 || roles_count > kMaxAllowedMentionIds) {
		pContext->ReportError("Invalid allowed mention role count %d (must be 0-%d)", roles_count, kMaxAllowedMentionIds);
		return 0;
	}

	cell_t* users_array = nullptr;
	cell_t* roles_array = nullptr;

	if (users_count > 0 && pContext->LocalToPhysAddr(params[3], &users_array) != SP_ERROR_NONE) {
		pContext->ReportError("Invalid allowed mention users array");
		return 0;
	}

	if (roles_count > 0 && pContext->LocalToPhysAddr(params[5], &roles_array) != SP_ERROR_NONE) {
		pContext->ReportError("Invalid allowed mention roles array");
		return 0;
	}

	std::vector<dpp::snowflake> users(users_count);
	std::vector<dpp::snowflake> roles(roles_count);

	for (size_t i = 0; i < users.size(); i++) {
		char* str;
		if (pContext->LocalToString(users_array[i], &str) != SP_ERROR_NONE || !ParseSnowflake(pContext, str, users[i])) {
			return 0;
		}
	}

	for (size_t i = 0; i < roles.size(); i++) {
		char* str;
		if (pContext->LocalToString(roles_array[i], &str) != SP_ERROR_NONE || !ParseSnowflake(pContext, str, roles[i])) {
			return 0;
		}
	}

	message->SetAllowedMentions(params[2], users, roles);
	return 1;
}

static cell_t message_AddComponent(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordComponent* component = Handles.GetPointer<DiscordComponent>(pContext, params[2]);
	if (!component) return 0;

	message->AddComponent(component);
	return 1;
}

static cell_t message_ClearComponents(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	message->ClearComponents();
	return 1;
}

static cell_t message_GetAttachment(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordAttachment* attachment = message->GetAttachment(params[2]);
	if (!attachment) return 0;

	Handle_t handle = Handles.Create(pContext, attachment, HandleId::DiscordAttachment);
	if (!handle) return 0;
	return handle;
}

static cell_t message_GetReaction(IPluginContext* pContext, const cell_t* params)
{
	DiscordMessage* message = Handles.GetPointer<DiscordMessage>(pContext, params[1]);
	if (!message) return 0;

	DiscordReaction* reaction = message->GetReaction(params[2]);
	if (!reaction) return 0;

	Handle_t handle = Handles.Create(pContext, reaction, HandleId::DiscordReaction);
	if (!handle) return 0;
	return handle;
}

extern const sp_nativeinfo_t message_natives[] = {
	{"DiscordMessage.DiscordMessage", message_CreateFromId},
	{"DiscordMessage.CreateEmpty", message_CreateEmpty},
	{"DiscordMessage.CreateWithContent", message_CreateWithContent},
	{"DiscordMessage.CreateWithChannel", message_CreateWithChannel},
	{"DiscordMessage.CreateWithEmbed", message_CreateWithEmbed},
	{"DiscordMessage.CreateWithChannelEmbed", message_CreateWithChannelEmbed},
	{"DiscordMessage.FetchMessage", message_FetchMessage},
	{"DiscordMessage.GetContent", message_GetContent},
	{"DiscordMessage.ContentLength.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetContentLength>},
	{"DiscordMessage.GetMessageId", message_GetMessageId},
	{"DiscordMessage.GetChannelId", message_GetChannelId},
	{"DiscordMessage.GetGuildId", EntityGetGuildId<DiscordMessage>},
	{"DiscordMessage.Author.get", message_GetAuthor},
	{"DiscordMessage.GetAuthorNickName", message_GetAuthorNickName},
	{"DiscordMessage.Type.get", message_GetType},
	{"DiscordMessage.Type.set", message_SetType},
	{"DiscordMessage.IsPinned.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsPinned>},
	{"DiscordMessage.IsTTS.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsTTS>},
	{"DiscordMessage.IsTTS.set", message_SetTTS},
	{"DiscordMessage.IsMentionEveryone.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsMentionEveryone>},
	{"DiscordMessage.IsBot.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsBot>},
	{"DiscordMessage.Edit", message_Edit},
	{"DiscordMessage.EditEmbed", message_EditEmbed},
	{"DiscordMessage.EditFromObject", message_EditFromObject},
	{"DiscordMessage.Delete", message_Delete},
	{"DiscordMessage.Pin", message_Pin},
	{"DiscordMessage.Unpin", message_Unpin},
	{"DiscordMessage.AddReaction", message_AddReaction},
	{"DiscordMessage.RemoveReaction", message_RemoveReaction},
	{"DiscordMessage.RemoveAllReactions", message_RemoveAllReactions},
	{"DiscordMessage.Reply", message_Reply},
	{"DiscordMessage.ReplyEmbed", message_ReplyEmbed},
	{"DiscordMessage.ReplyFromObject", message_ReplyFromObject},
	{"DiscordMessage.Crosspost", message_Crosspost},
	{"DiscordMessage.CreateThread", message_CreateThread},
	{"DiscordMessage.Flags.get", EntityGetFlags<DiscordMessage>},
	{"DiscordMessage.Flags.set", message_SetFlags},
		{"DiscordMessage.GetTimestamp", message_GetTimestamp},
		{"DiscordMessage.GetEditedTimestamp", message_GetEditedTimestamp},
	{"DiscordMessage.IsDM.get", EntityGetBool<DiscordMessage, &DiscordMessage::IsDM>},
	{"DiscordMessage.HasRemixAttachment.get", EntityGetBool<DiscordMessage, &DiscordMessage::HasRemixAttachment>},
	{"DiscordMessage.HasReference.get", EntityGetBool<DiscordMessage, &DiscordMessage::HasReference>},
	{"DiscordMessage.GetReferencedMessageId", message_GetReferencedMessageId},
	{"DiscordMessage.GetReferencedChannelId", message_GetReferencedChannelId},
	{"DiscordMessage.GetReferencedGuildId", message_GetReferencedGuildId},
	{"DiscordMessage.AttachmentCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetAttachmentCount>},
	{"DiscordMessage.EmbedCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetEmbedCount>},
	{"DiscordMessage.ReactionCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetReactionCount>},
	{"DiscordMessage.StickerCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetStickerCount>},
	{"DiscordMessage.MentionedUserCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetMentionedUserCount>},
	{"DiscordMessage.MentionedRoleCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetMentionedRoleCount>},
	{"DiscordMessage.MentionedChannelCount.get", EntityGetInt<DiscordMessage, size_t, &DiscordMessage::GetMentionedChannelCount>},
	{"DiscordMessage.GetMentionedUserId", message_GetMentionedUserId},
	{"DiscordMessage.GetMentionedRoleId", message_GetMentionedRoleId},
	{"DiscordMessage.GetMentionedChannelId", message_GetMentionedChannelId},
	{"DiscordMessage.GetMentionedUserIds", message_GetMentionedUserIds},
	{"DiscordMessage.GetMentionedRoleIds", message_GetMentionedRoleIds},
	{"DiscordMessage.GetMentionedChannelIds", message_GetMentionedChannelIds},
	{"DiscordMessage.GetWebhookId", message_GetWebhookId},
	{"DiscordMessage.GetNonce", message_GetNonce},
	{"DiscordMessage.GetUrl", message_GetURL},
	{"DiscordMessage.SetContent", message_SetContent},
	{"DiscordMessage.SetChannelId", message_SetChannelId},
	{"DiscordMessage.SetNonce", message_SetNonce},
	{"DiscordMessage.SetAllowedMentions", message_SetAllowedMentions},
	{"DiscordMessage.AddEmbed", message_AddEmbed},
	{"DiscordMessage.AddFile", message_AddFile},
	{"DiscordMessage.ClearEmbeds", message_ClearEmbeds},
	{"DiscordMessage.AddComponent", message_AddComponent},
	{"DiscordMessage.ClearComponents", message_ClearComponents},
	{"DiscordMessage.Send", message_Send},
	{"DiscordMessage.HasPoll.get", EntityGetBool<DiscordMessage, &DiscordMessage::HasPoll>},
	{"DiscordMessage.SetPoll", message_SetPoll},
	{"DiscordMessage.Poll.get", message_GetPoll},
	{"DiscordMessage.EndPoll", message_EndPoll},
	{"DiscordMessage.GetPollAnswerVoters", message_GetPollAnswerVoters},
	{"DiscordMessage.GetAttachment", message_GetAttachment},
	{"DiscordMessage.GetReaction", message_GetReaction},
	{nullptr, nullptr}
};
