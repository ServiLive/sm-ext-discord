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

#pragma once

#include "core/discord_client_ref.h"
#include "utils/discord_embed.h"
#include "utils/discord_poll.h"
#include "discord_user.h"
#include "discord_attachment.h"
#include "discord_reaction.h"
#include "core/handle_manager.h"

class DiscordMessage
{
private:
	dpp::message m_message;
	DiscordClientRef m_client;
	mutable CachedHandle<DiscordUser> m_authorHandle;
	mutable CachedHandle<DiscordPoll> m_pollHandle;
	size_t m_pendingUploadBytes = 0;
	size_t m_pendingUploadCount = 0;

public:
	DiscordMessage(const dpp::message& msg, DiscordClient* client) : m_message(msg), m_client(client) {}
	DiscordMessage(DiscordClient* client) : m_client(client) {} // Empty message
	DiscordMessage(const char* content, DiscordClient* client) : m_client(client) {
		m_message.content = content;
	}
	DiscordMessage(dpp::snowflake channel_id, const char* content, DiscordClient* client) : m_client(client) {
		m_message.channel_id = channel_id;
		m_message.content = content;
	}
	DiscordMessage(const DiscordEmbed* embed, DiscordClient* client) : m_client(client) {
		if (embed) m_message.add_embed(embed->GetEmbed());
	}
	DiscordMessage(dpp::snowflake channel_id, const DiscordEmbed* embed, DiscordClient* client) : m_client(client) {
		m_message.channel_id = channel_id;
		if (embed) m_message.add_embed(embed->GetEmbed());
	}

	Handle_t GetAuthorHandle() const {
		return m_authorHandle.GetOrCreate([this]() { return GetAuthor(); });
	}

	DiscordUser* GetAuthor() const {
		if (!m_client) return nullptr;
		return new DiscordUser(m_message.author, m_client.Get());
	}
	const char* GetContent() const { return m_message.content.c_str(); }
	size_t GetContentLength() const { return m_message.content.length(); }
	std::string GetMessageId() const { return m_message.id.str(); }
	std::string GetChannelId() const { return m_message.channel_id.str(); }
	std::string GetGuildId() const { return m_message.guild_id.str(); }
	std::string GetAuthorNickName() const { return m_message.member.get_nickname(); }
	dpp::message_type GetType() const { return m_message.type; }
	bool IsPinned() const { return m_message.pinned; }
	bool IsTTS() const { return m_message.tts; }
	bool IsMentionEveryone() const { return m_message.mention_everyone; }
	bool IsBot() const { return m_message.author.is_bot(); }

	// Message flags
	uint16_t GetFlags() const { return m_message.flags; }

	// Additional properties
	std::string GetWebhookId() const { return m_message.webhook_id.str(); }
	time_t GetTimestamp() const { return m_message.sent; }
	time_t GetEditedTimestamp() const { return m_message.edited; }
	const char* GetNonce() const { return m_message.nonce.c_str(); }
	bool IsDM() const { return m_message.guild_id == 0; }
	bool HasReference() const { return m_message.message_reference.message_id != 0; }
	std::string GetReferencedMessageId() const { return m_message.message_reference.message_id.str(); }
	std::string GetReferencedChannelId() const { return m_message.message_reference.channel_id.str(); }
	std::string GetReferencedGuildId() const { return m_message.message_reference.guild_id.str(); }

	bool HasRemixAttachment() const {
		for (const auto& attachment : m_message.attachments) {
			if (attachment.flags & (1 << 2)) return true; // a_is_remix
		}
		return false;
	}
	std::string GetURL() const {
		if (m_message.guild_id != 0) {
			return "https://discord.com/channels/" + m_message.guild_id.str() + "/" +
				m_message.channel_id.str() + "/" + m_message.id.str();
		} else {
			return "https://discord.com/channels/@me/" + m_message.channel_id.str() + "/" +
				m_message.id.str();
		}
	}

	// Collection counts
	size_t GetAttachmentCount() const { return m_message.attachments.size(); }
	size_t GetEmbedCount() const { return m_message.embeds.size(); }
	size_t GetReactionCount() const { return m_message.reactions.size(); }
	size_t GetStickerCount() const { return m_message.stickers.size(); }
	size_t GetMentionedUserCount() const { return m_message.mentions.size(); }
	size_t GetMentionedRoleCount() const { return m_message.mention_roles.size(); }
	size_t GetMentionedChannelCount() const { return m_message.mention_channels.size(); }

	// Object accessors
	DiscordAttachment* GetAttachment(size_t index) const {
		if (index >= m_message.attachments.size()) return nullptr;
		return new DiscordAttachment(m_message.attachments[index]);
	}
	DiscordReaction* GetReaction(size_t index) const {
		if (index >= m_message.reactions.size()) return nullptr;
		return new DiscordReaction(m_message.reactions[index]);
	}

	// Collection accessors
	std::string GetMentionedUserId(size_t index) const {
		if (index >= m_message.mentions.size()) return "";
		return m_message.mentions[index].first.id.str();
	}
	std::string GetMentionedRoleId(size_t index) const {
		if (index >= m_message.mention_roles.size()) return "";
		return m_message.mention_roles[index].str();
	}
	std::string GetMentionedChannelId(size_t index) const {
		if (index >= m_message.mention_channels.size()) return "";
		return m_message.mention_channels[index].id.str();
	}

	// Array accessors
	const std::vector<std::pair<dpp::user, dpp::guild_member>>& GetMentionedUsers() const { return m_message.mentions; }
	const std::vector<dpp::snowflake>& GetMentionedRoles() const { return m_message.mention_roles; }
	const std::vector<dpp::channel>& GetMentionedChannels() const { return m_message.mention_channels; }

	// Message actions
	void Reply(const char* content, IPluginFunction* callback = nullptr, cell_t data = 0);
	void ReplyEmbed(const char* content, const class DiscordEmbed* embed, IPluginFunction* callback = nullptr, cell_t data = 0);
	void ReplyFromObject(const DiscordMessage* reply_message, IPluginFunction* callback = nullptr, cell_t data = 0);
	void Crosspost();
	void CreateThread(const char* name, uint16_t auto_archive_duration = 60, IPluginFunction* callback = nullptr, cell_t data = 0);

	// Message management methods
	void Edit(const char* new_content, IPluginFunction* callback = nullptr, cell_t data = 0);
	void EditEmbed(const char* new_content, const class DiscordEmbed* embed, IPluginFunction* callback = nullptr, cell_t data = 0);
	void EditFromObject(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Delete(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Pin(IPluginFunction* callback = nullptr, cell_t data = 0);
	void Unpin(IPluginFunction* callback = nullptr, cell_t data = 0);
	void AddReaction(const char* emoji);
	void RemoveReaction(const char* emoji);
	void RemoveAllReactions();

	// Message property setters
	void SetContent(const char* content) { m_message.content = content; }
	void SetChannelId(dpp::snowflake channel_id) { m_message.channel_id = channel_id; }
	void SetType(dpp::message_type type) { m_message.type = type; }
	void SetFlags(uint16_t flags) { m_message.flags = flags; }
	void SetTTS(bool tts) { m_message.tts = tts; }
	void SetNonce(const char* nonce) { m_message.nonce = nonce; }
	void SetAllowedMentions(int allowed_mentions_mask, const std::vector<dpp::snowflake>& users, const std::vector<dpp::snowflake>& roles) {
		m_message.set_allowed_mentions(
			allowed_mentions_mask & 1,
			allowed_mentions_mask & 2,
			allowed_mentions_mask & 4,
			allowed_mentions_mask & 8,
			users,
			roles
		);
	}
	void AddEmbed(const class DiscordEmbed* embed) {
		if (embed) m_message.add_embed(embed->GetEmbed());
	}
	bool AddFile(const std::string& filename, const std::string& content, const std::string& mimetype) {
		static constexpr size_t kMaxUploadFiles = 10;
		static constexpr size_t kMaxUploadBytes = 25u * 1024u * 1024u;

		if (filename.empty() || content.empty() || m_pendingUploadCount >= kMaxUploadFiles) return false;
		if (content.size() > kMaxUploadBytes - m_pendingUploadBytes) return false;

		m_message.add_file(filename, content, mimetype.empty() ? "application/octet-stream" : mimetype);
		m_pendingUploadBytes += content.size();
		m_pendingUploadCount++;
		return true;
	}
	void ClearEmbeds() { m_message.embeds.clear(); }
	void AddComponent(const class DiscordComponent* component);
	void ClearComponents() { m_message.components.clear(); }
	void Send(IPluginFunction* callback = nullptr, cell_t data = 0); // Send this message to its channel

	// Poll support
	bool HasPoll() const { return m_message.has_poll(); }
	void SetPoll(const class DiscordPoll* poll);
	Handle_t GetPollHandle() const {
		return m_pollHandle.GetOrCreate([this]() { return GetPollInternal(); });
	}
	DiscordPoll* GetPollInternal() const;
	void EndPoll();
	bool GetPollAnswerVoters(uint32_t answer_id, IPluginFunction* callback, cell_t data = 0);

	// Internal accessor
	const dpp::message& GetDPPMessage() const { return m_message; }
};
