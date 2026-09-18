// Discord message
methodmap DiscordMessage < Handle
{
  /**
   * Creates a DiscordMessage object from message and channel IDs
   *
   * @param discord     Discord client handle to bind to this message
   * @param message_id  Message ID as string
   * @param channel_id  Channel ID as string
   * @return            DiscordMessage handle or null on failure
   * @note              This creates a minimal message object. Use FetchMessage for complete info
   */
  public native DiscordMessage(Discord discord, const char[] message_id, const char[] channel_id);

  /**
   * Creates an empty DiscordMessage object
   *
   * @param discord     Discord client handle to bind to this message
   * @return            DiscordMessage handle or null on failure
   */
  public static native DiscordMessage CreateEmpty(Discord discord);

  /**
   * Creates a DiscordMessage object with content
   *
   * @param discord     Discord client handle
   * @param content     Message content
   * @return            DiscordMessage handle or null on failure
   */
  public static native DiscordMessage CreateWithContent(Discord discord, const char[] content);

  /**
   * Creates a DiscordMessage object with channel and content
   *
   * @param discord     Discord client handle
   * @param channel_id  Channel ID as string
   * @param content     Message content
   * @return            DiscordMessage handle or null on failure
   */
  public static native DiscordMessage CreateWithChannel(Discord discord, const char[] channel_id, const char[] content);

  /**
   * Creates a DiscordMessage object with an embed
   *
   * @param discord     Discord client handle
   * @param embed       DiscordEmbed handle
   * @return            DiscordMessage handle or null on failure
   */
  public static native DiscordMessage CreateWithEmbed(Discord discord, DiscordEmbed embed);

  /**
   * Creates a DiscordMessage object with channel and embed
   *
   * @param discord     Discord client handle
   * @param channel_id  Channel ID as string
   * @param embed       DiscordEmbed handle
   * @return            DiscordMessage handle or null on failure
   */
  public static native DiscordMessage CreateWithChannelEmbed(Discord discord, const char[] channel_id, DiscordEmbed embed);

  /**
   * Fetches full message information from Discord API asynchronously
   *
   * @param discord     Discord client handle
   * @param message_id  Message ID as string
   * @param channel_id  Channel ID as string
   * @param callback    Function to call when message info is received
   * @param data        Optional data to pass to callback
   * @note              Available information: content, author, timestamp, embeds, reactions, etc.
   * @note              This method makes an API request to fetch message data. Messages are not cached
   *                    by the Discord library due to memory constraints and privacy considerations
   * @note              Result contains: "message" (DiscordMessage handle)
   */
  public static native void FetchMessage(Discord discord, const char[] message_id, const char[] channel_id, DiscordResultCallback callback, any data = 0);

  /**
   * Gets the content of the message
   *
   * @param buffer      Buffer to store the content
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetContent(char[] buffer, int maxlength);

  /**
   * Gets the ID of the message
   *
   * @param buffer      Buffer to store the message ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetMessageId(char[] buffer, int maxlength);

  /**
   * Gets the channel ID where the message was sent
   *
   * @param buffer      Buffer to store the channel ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetChannelId(char[] buffer, int maxlength);

  /**
   * Gets the guild (server) ID where the message was sent
   *
   * @param buffer      Buffer to store the guild ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetGuildId(char[] buffer, int maxlength);

  /**
   * Gets the user that sent the message
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordUser Author {
    public native get();
  }

  /**
   * Gets the nickname of the message author within the guild this message was sent in
   *
   * @param buffer      Buffer to store the author nickname
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetAuthorNickName(char[] buffer, int maxlength);

  /**
   * Checks if the message is pinned
   *
   * @return          True if pinned, false otherwise
   */
  property bool IsPinned {
    public native get();
  }

  /**
   * Checks or sets whether the message is TTS (Text-To-Speech)
   *
   * @return          True if TTS, false otherwise
   */
  property bool IsTTS {
    public native get();
    public native set(bool tts)
  }

  /**
   * Checks if the message mentions everyone
   *
   * @return          True if mentions everyone, false otherwise
   */
  property bool IsMentionEveryone {
    public native get();
  }

  /**
   * Checks if the message author is a bot
   *
   * @return          True if author is bot, false otherwise
   */
  property bool IsBot {
    public native get();
  }

  /**
   * Gets the UTF-8 byte length of the message content
   * @note            This is the encoded string length, not the number of Unicode code points
   *                  or user-visible characters. It does not include the null terminator
   *
   * @return          UTF-8 byte length of the message content
   */
  property int ContentLength {
    public native get();
  }

  /**
   * Gets or sets message type
   *
   * @return          Message type
   */
  property DiscordMessageType Type {
    public native get();
    public native set(DiscordMessageType type);
  }

  /**
   * Edits this message content
   *
   * @param content   New message content
   * @param callback  Optional callback to receive the edited message
   * @param data      Optional data to pass to callback
   */
  public native void Edit(const char[] content, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Edits this message with embed
   *
   * @param content   New message content
   * @param embed     Embed object
   * @param callback  Optional callback to receive the edited message
   * @param data      Optional data to pass to callback
   */
  public native void EditEmbed(const char[] content, DiscordEmbed embed, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Edits this message using the full message object
   *
   * @note            Use this method when you need to edit components, embeds, or other properties
   * @note            Modify the message properties (SetContent, AddEmbed, AddComponent, etc.) before calling this
   * @param callback  Optional callback to receive the edited message
   * @param data      Optional data to pass to callback
   */
  public native void EditFromObject(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Deletes this message
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Delete(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Pins this message
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Pin(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Unpins this message
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Unpin(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Adds a reaction to this message
   *
   * @param emoji     Emoji to add (e.g., "👍" or "emoji_name:123456789")
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void AddReaction(const char[] emoji, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Removes a reaction from this message
   *
   * @param emoji     Emoji to remove
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void RemoveReaction(const char[] emoji, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Removes all reactions from this message
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void RemoveAllReactions(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Gets or sets the message flags
   */
  property DiscordMessageFlags Flags {
    public native get();
    public native set(DiscordMessageFlags flags);
  }

 /**
   * Gets when this message was sent as a Unix timestamp string
   *
   * @param buffer      Buffer to store the timestamp
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetTimestamp(char[] buffer, int maxlength);

 /**
   * Gets when this message was edited as a Unix timestamp string
   *
   * @param buffer      Buffer to store the timestamp
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetEditedTimestamp(char[] buffer, int maxlength);

  /**
   * Gets the nonce used for message validation
   *
   * @param buffer      Buffer to store the nonce
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetNonce(char[] buffer, int maxlength);

  /**
   * Checks if the message is from a DM
   *
   * @return          True if DM, false otherwise
   */
  property bool IsDM {
    public native get();
  }

  /**
   * Checks if message has remixed attachment
   *
   * @return          True if has remixed attachment, false otherwise
   */
  property bool HasRemixAttachment {
    public native get();
  }

  /**
   * Checks if message has a reference (reply/forward)
   *
   * @return          True if has reference, false otherwise
   */
  property bool HasReference {
    public native get();
  }

  /**
   * Gets the referenced message ID
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetReferencedMessageId(char[] buffer, int maxlength);

  /**
   * Gets the referenced message's channel ID
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetReferencedChannelId(char[] buffer, int maxlength);

  /**
   * Gets the referenced message's guild ID (for cross-guild references)
   *
   * @param buffer      Buffer to store the ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetReferencedGuildId(char[] buffer, int maxlength);

  /**
   * Gets the URL to this message
   *
   * @param buffer      Buffer to store the URL
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetUrl(char[] buffer, int maxlength);

  /**
   * Gets the number of attachments
   */
  property int AttachmentCount {
    public native get();
  }

  /**
   * Gets the number of embeds
   */
  property int EmbedCount {
    public native get();
  }

  /**
   * Gets the number of reactions
   */
  property int ReactionCount {
    public native get();
  }

  /**
   * Gets the number of stickers
   */
  property int StickerCount {
    public native get();
  }

  /**
   * Gets the number of mentioned users
   */
  property int MentionedUserCount {
    public native get();
  }

  /**
   * Gets the number of mentioned roles
   */
  property int MentionedRoleCount {
    public native get();
  }

  /**
   * Gets the number of mentioned channels
   */
  property int MentionedChannelCount {
    public native get();
  }

  /**
   * Gets mentioned user ID by index
   *
   * @param index     Index of the mentioned user
   * @param buffer    Buffer to store the user ID
   * @param maxlength Maximum length of the buffer
   */
  public native void GetMentionedUserId(int index, char[] buffer, int maxlength);

  /**
   * Gets mentioned role ID by index
   *
   * @param index     Index of the mentioned role
   * @param buffer    Buffer to store the role ID
   * @param maxlength Maximum length of the buffer
   */
  public native void GetMentionedRoleId(int index, char[] buffer, int maxlength);

  /**
   * Gets mentioned channel ID by index
   *
   * @param index     Index of the mentioned channel
   * @param buffer    Buffer to store the channel ID
   * @param maxlength Maximum length of the buffer
   */
  public native void GetMentionedChannelId(int index, char[] buffer, int maxlength);

  /**
   * Gets all mentioned user IDs as an array
   *
   * @return          DiscordHandleArray containing all mentioned user IDs
   * @note            Caller is responsible for closing the returned handle
   */
  public native DiscordHandleArray GetMentionedUserIds();

  /**
   * Gets all mentioned role IDs as an array
   *
   * @return          DiscordHandleArray containing all mentioned role IDs
   * @note            Caller is responsible for closing the returned handle
   */
  public native DiscordHandleArray GetMentionedRoleIds();

  /**
   * Gets all mentioned channel IDs as an array
   *
   * @return          DiscordHandleArray containing all mentioned channel IDs
   * @note            Caller is responsible for closing the returned handle
   */
  public native DiscordHandleArray GetMentionedChannelIds();

  /**
   * Gets the webhook ID if message was sent by webhook
   *
   * @param buffer      Buffer to store the webhook ID
   * @param maxlength   Maximum length of the buffer
   */
  public native void GetWebhookId(char[] buffer, int maxlength);

  /**
   * Sets the message content
   *
   * @param content     New message content
   */
  public native void SetContent(const char[] content);

  /**
   * Sets the channel ID for this message
   *
   * @param channel_id  Channel ID as string
   */
  public native void SetChannelId(const char[] channel_id);

  /**
   * Sets the nonce for message validation
   *
   * @param nonce       Nonce string
   */
  public native void SetNonce(const char[] nonce);

  /**
   * Sets allowed mentions for this message
   *
   * @param allowedMentionsMask    Allowed mentions mask
   * @param allowedUsersMentions   Optional array of allowed user IDs to mention
   * @param allowedUserSize        Size of allowed users array (default: 0)
   * @param allowedRolesMentions   Optional array of allowed role IDs to mention
   * @param allowedRolesSize       Size of allowed roles array (default: 0)
   */
  public native void SetAllowedMentions(DiscordAllowedMentions allowedMentionsMask = AllowedMentions_None, const char[][] allowedUsersMentions = {}, int allowedUserSize = 0, const char[][] allowedRolesMentions = {}, int allowedRolesSize = 0);

  /**
   * Adds an embed to this message
   *
   * @param embed       DiscordEmbed handle to add
   */
  public native void AddEmbed(DiscordEmbed embed);

  /**
   * Adds a local file to this outgoing message.
   *
   * @param path       File path relative to the game directory.
   * @param filename   Optional filename shown by Discord. If empty, the basename of path is used.
   * @param mimetype   Optional MIME type. If empty, application/octet-stream is used.
   * @return           True if the file was read and attached successfully.
   * @note             Absolute paths and parent-directory traversal (..) are rejected.
   * @note             This wrapper allows up to 10 files and 25 MiB total per message.
   */
  public native bool AddFile(const char[] path, const char[] filename = "", const char[] mimetype = "");

  /**
   * Clears all embeds from this message
   */
  public native void ClearEmbeds();

  /**
   * Adds a component (action row) to this message
   *
   * @param component   DiscordComponent handle (typically an action row containing buttons/selects)
   * @note              Components must be wrapped in action rows before adding to messages
   */
  public native void AddComponent(DiscordComponent component);

  /**
   * Clears all components from this message
   */
  public native void ClearComponents();

  /**
   * Sends this message to its channel
   *
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   */
  public native void Send(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates a reply to this message
   *
   * @param content   Message content to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   */
  public native void Reply(const char[] content, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates a reply to this message with embed
   *
   * @param content   Message content to send
   * @param embed     Embed object to send
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   */
  public native void ReplyEmbed(const char[] content, DiscordEmbed embed, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates a reply to this message using a message object
   *
   * @param message   DiscordMessage object containing the reply content
   * @param callback  Optional callback to receive the created message
   * @param data      Optional data to pass to callback
   * @note            Use this method when you need to include components, multiple embeds, or other advanced features
   */
  public native void ReplyFromObject(DiscordMessage message, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Crossposts this message (publish to following channels)
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void Crosspost(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Creates a thread from this message
   *
   * @param name                  Name of the thread
   * @param auto_archive_duration Duration after which thread auto-archives (AutoArchive_1Hour, AutoArchive_1Day, AutoArchive_3Days, AutoArchive_1Week)
   * @param callback              Optional callback to receive the created thread
   * @param data                  Optional data to pass to callback
   */
  public native void CreateThread(const char[] name, DiscordAutoArchiveDuration auto_archive_duration = AutoArchive_1Hour, DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Checks if this message has a poll
   *
   * @return          True if message has a poll, false otherwise
   */
  property bool HasPoll {
    public native get();
  }

  /**
   * Sets a poll for this message (before sending)
   *
   * @param poll      DiscordPoll handle to attach
   */
  public native void SetPoll(DiscordPoll poll);

  /**
   * Gets the poll attached to this message
   * @note Automatically managed - no need to manually free this handle
   */
  property DiscordPoll Poll {
    public native get();
  }

  /**
   * Ends the poll on this message early
   *
   * @param callback  Optional callback to receive confirmation
   * @param data      Optional data to pass to callback
   */
  public native void EndPoll(DiscordResultCallback callback = INVALID_FUNCTION, any data = 0);

  /**
   * Gets the users who voted for a specific poll answer
   *
   * @param answer_id   Discord poll answer ID, usually from DiscordPoll.GetAnswerId()
   * @param callback    Callback function to receive the voters
   * @param data        Optional data to pass to callback
   * @note              Result contains: "items" (DiscordHandleArray of DiscordUser), "count" (int)
   */
  public native void GetPollAnswerVoters(int answer_id, DiscordResultCallback callback, any data = 0);

  /**
   * Gets a attachment object by index
   *
   * @param index       Index of the attachment (0 to AttachmentCount-1)
   * @return            DiscordAttachment handle or null if index out of bounds
   * @note              Caller is responsible for closing the returned handle
   */
  public native DiscordAttachment GetAttachment(int index);

  /**
   * Gets a reaction object by index
   *
   * @param index       Index of the reaction (0 to ReactionCount-1)
   * @return            DiscordReaction handle or null if index out of bounds
   * @note              Caller is responsible for closing the returned handle
   */
  public native DiscordReaction GetReaction(int index);
}
