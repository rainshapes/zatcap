#include <memory.h>
#include "twitcurl.h"
#include "urlencode.h"

/*++
* @method: twitCurl::twitCurl
*
* @description: constructor
*
* @input: none
*
* @output: none
*
*--*/
twitCurl::twitCurl():
m_eApiFormatType( twitCurlTypes::eTwitCurlApiFormatXml )
{
    /* Clear callback buffers */
    clearCurlCallbackBuffers();
	handleInUse=0;
	m_curlHandle=curl_easy_init();
}

/*++
* @method: twitCurl::~twitCurl
*
* @description: destructor
*
* @input: none
*
* @output: none
*
*--*/
twitCurl::~twitCurl()
{
}

/*++
* @method: twitCurl::setTwitterApiType
*
* @description: method to set API type
*
* @input: none
*
* @output: none
*
*--*/
void twitCurl::setTwitterApiType( twitCurlTypes::eTwitCurlApiFormatType eType )
{
    m_eApiFormatType = ( eType < twitCurlTypes::eTwitCurlApiFormatMax ) ?
                        eType : twitCurlTypes::eTwitCurlApiFormatXml;
}


/*++
* @method: twitCurl::getTwitterUsername
*
* @description: method to get stored Twitter username
*
* @input: none
*
* @output: twitter username
*
*--*/
std::string& twitCurl::getTwitterUsername()
{
    return m_twitterUsername;
}

/*++
* @method: twitCurl::getTwitterPassword
*
* @description: method to get stored Twitter password
*
* @input: none
*
* @output: twitter password
*
*--*/
std::string& twitCurl::getTwitterPassword()
{
    return m_twitterPassword;
}

/*++
* @method: twitCurl::setTwitterUsername
*
* @description: method to set username
*
* @input: userName
*
* @output: none
*
*--*/
void twitCurl::setTwitterUsername( std::string& userName )
{
    if( userName.length() )
    {
        m_twitterUsername = userName;
    }
}

/*++
* @method: twitCurl::setTwitterPassword
*
* @description: method to set password
*
* @input: passWord
*
* @output: none
*
*--*/
void twitCurl::setTwitterPassword( std::string& passWord )
{
    if( passWord.length() )
    {
        m_twitterPassword = passWord;
//        m_curlLoginParamsSet = false;
    }
}

/*++
* @method: twitCurl::getProxyServerIp
*
* @description: method to get proxy server IP address
*
* @input: none
*
* @output: proxy server IP address
*
*--*/
std::string& twitCurl::getProxyServerIp()
{
    return m_proxyServerIp;
}

/*++
* @method: twitCurl::getProxyServerPort
*
* @description: method to get proxy server port
*
* @input: none
*
* @output: proxy server port
*
*--*/
std::string& twitCurl::getProxyServerPort()
{
    return m_proxyServerPort;
}

/*++
* @method: twitCurl::getProxyUserName
*
* @description: method to get proxy user name
*
* @input: none
*
* @output: proxy server user name
*
*--*/
std::string& twitCurl::getProxyUserName()
{
    return m_proxyUserName;
}

/*++
* @method: twitCurl::getProxyPassword
*
* @description: method to get proxy server password
*
* @input: none
*
* @output: proxy server password
*
*--*/
std::string& twitCurl::getProxyPassword()
{
    return m_proxyPassword;
}

/*++
* @method: twitCurl::setProxyServerIp
*
* @description: method to set proxy server IP address
*
* @input: proxyServerIp
*
* @output: none
*
*--*/
void twitCurl::setProxyServerIp( std::string& proxyServerIp )
{
    if( proxyServerIp.length() )
    {
        m_proxyServerIp = proxyServerIp;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
    }
}

/*++
* @method: twitCurl::setProxyServerPort
*
* @description: method to set proxy server port
*
* @input: proxyServerPort
*
* @output: none
*
*--*/
void twitCurl::setProxyServerPort( std::string& proxyServerPort )
{
    if( proxyServerPort.length() )
    {
        m_proxyServerPort = proxyServerPort;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
    }
}

/*++
* @method: twitCurl::setProxyUserName
*
* @description: method to set proxy server username
*
* @input: proxyUserName
*
* @output: none
*
*--*/
void twitCurl::setProxyUserName( std::string& proxyUserName )
{
    if( proxyUserName.length() )
    {
        m_proxyUserName = proxyUserName;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
    }
}

/*++
* @method: twitCurl::setProxyPassword
*
* @description: method to set proxy server password
*
* @input: proxyPassword
*
* @output: none
*
*--*/
void twitCurl::setProxyPassword( std::string& proxyPassword )
{
    if( proxyPassword.length() )
    {
        m_proxyPassword = proxyPassword;
        /*
         * Reset the flag so that next cURL http request
         * would set proxy details again into cURL.
         */
//        m_curlProxyParamsSet = false;
    }
}

/*++
* @method: twitCurl::search
*
* @description: method to return tweets that match a specified query.
*
* @input: searchQuery - search query in string format
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
* @note: Only ATOM and JSON format supported.
*
*--*/
std::string twitCurl::search( std::string& searchQuery )
{
    /* Prepare URL */
    std::string buildUrl = twitterDefaults::TWITCURL_SEARCH_URL +
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[twitCurlTypes::eTwitCurlApiFormatJson] +
                           twitCurlDefaults::TWITCURL_URL_SEP_QUES + twitCurlDefaults::TWITCURL_SEARCHQUERYSTRING +
                           searchQuery;

    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::statusUpdate
*
* @description: method to update new status message in twitter profile
*
* @input: newStatus
*
* @output: true if POST is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::statusUpdate( std::string& newStatus,std::string inReplyTo )
{
    std::string retVal = "";
    if( newStatus.length() )
    {
        /* Prepare new status message */
        std::string newStatusMsg = twitCurlDefaults::TWITCURL_STATUSSTRING + urlencode( newStatus );
		if(inReplyTo.size())
		{
			//newStatusMsg+=std::string("?in_reply_to_status_id=")+inReplyTo+"";
			retVal = performPost( twitterDefaults::TWITCURL_STATUSUPDATE_URL +
				twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+std::string("?in_reply_to_status_id=")+inReplyTo,
				newStatusMsg );
		}
		else
        /* Perform POST */
        retVal = performPost( twitterDefaults::TWITCURL_STATUSUPDATE_URL +
                              twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                              newStatusMsg );
    }
    return retVal;
}

/*++
* @method: twitCurl::statusShowById
*
* @description: method to get a status message by its id
*
* @input: statusId - a number in std::string format
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::statusShowById( std::string& statusId )
{
    std::string retVal = "";
    if( statusId.length() )
    {
        /* Prepare URL */
        std::string buildUrl = twitterDefaults::TWITCURL_STATUSSHOW_URL + 
                               twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+twitCurlDefaults::TWITCURL_URL_SEP_QUES+"id="+statusId +twitCurlDefaults::TWITCURL_URL_SEP_AMP+twitCurlDefaults::TWITCURL_INCLUDEENTITIES;

        /* Perform GET */
        retVal = performGet( buildUrl );
    }
    return retVal;
}

/*++
* @method: twitCurl::statusDestroyById
*
* @description: method to delete a status message by its id
*
* @input: statusId - a number in std::string format
*
* @output: true if DELETE is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::statusDestroyById( std::string& statusId )
{
    std::string retVal = "";
    if( statusId.length() )
    {
        /* Prepare URL */
        std::string buildUrl = twitterDefaults::TWITCURL_STATUDESTROY_URL + statusId +
                               twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

        /* Perform DELETE */
        retVal = performDelete( buildUrl );
    }
    return retVal;
}

/*++
* @method: twitCurl::timelinePublicGet
*
* @description: method to get public timeline
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::timelinePublicGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_PUBLIC_TIMELINE_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+twitCurlDefaults::TWITCURL_URL_SEP_QUES+twitCurlDefaults::TWITCURL_INCLUDEENTITIES );
}

/*++
* @method: twitCurl::featuredUsersGet
*
* @description: method to get featured users
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::featuredUsersGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_FEATURED_USERS_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::timelineFriendsGet
*
* @description: method to get friends timeline
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::timelineFriendsGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_FRIENDS_TIMELINE_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] +twitCurlDefaults::TWITCURL_URL_SEP_QUES+twitCurlDefaults::TWITCURL_INCLUDEENTITIES);
}

/*++
* @method: twitCurl::mentionsGet
*
* @description: method to get mentions
*
* @input: sinceId - String specifying since id parameter
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::mentionsGet( std::string sinceId,std::string maxId )
{
    std::string buildUrl = twitterDefaults::TWITCURL_MENTIONS_URL +
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+twitCurlDefaults::TWITCURL_URL_SEP_QUES+twitCurlDefaults::TWITCURL_INCLUDEENTITIES;
    if( sinceId.length() )
    {
        buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_SINCEID + sinceId;
    }
	if( maxId.length() )
	{
		buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_MAXID + maxId;
	}
    /* Perform GET */
    return performGet( buildUrl );
}
std::string twitCurl::retweetsGet()
{
	std::string buildUrl = twitterDefaults::TWITCURL_RETWEETTIMELINE_URL +
		twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+twitCurlDefaults::TWITCURL_URL_SEP_QUES+twitCurlDefaults::TWITCURL_INCLUDEENTITIES;
	/* Perform GET */
	return performGet( buildUrl );
}
/*++
* @method: twitCurl::timelineUserGet
*
* @description: method to get mentions
*
* @input: trimUser - Trim user name if true
*         tweetCount - Number of tweets to get. Max 200.
*         userInfo - screen name or user id in string format,
*         isUserId - true if userInfo contains an id
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::timelineHomeGet( bool trimUser, bool includeRetweets, unsigned int tweetCount , std::string sinceId ,std::string maxId)
{
	/* Prepare URL */
	std::string buildUrl=twitterDefaults::TWITCURL_HOME_TIMELINE_URL +
		twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+twitCurlDefaults::TWITCURL_URL_SEP_QUES+twitCurlDefaults::TWITCURL_INCLUDEENTITIES;
	if(!tweetCount)
		tweetCount=800;
	if( tweetCount )
	{
		if( tweetCount > twitCurlDefaults::MAX_TIMELINE_TWEET_COUNT )
		{
			tweetCount = twitCurlDefaults::MAX_TIMELINE_TWEET_COUNT;
		}
		std::stringstream tmpStrm;
		tmpStrm << twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_COUNT << tweetCount;
		buildUrl += tmpStrm.str();
		tmpStrm.str().clear();
	}

	if( includeRetweets )
	{
		buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_INCRETWEETS;
	}

	if( trimUser )
	{
		buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_TRIMUSER;
	}
	if( sinceId.length() )
	{
		buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_SINCEID + sinceId;
	}
	if(maxId.length() )
	{
		buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_MAXID + maxId;
	}

	/* Perform GET */
	return performGet( buildUrl );
}
/*++
* @method: twitCurl::timelineUserGet
*
* @description: method to get mentions
*
* @input: trimUser - Trim user name if true
*         tweetCount - Number of tweets to get. Max 200.
*         userInfo - screen name or user id in string format,
*         isUserId - true if userInfo contains an id
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::timelineUserGet( bool trimUser, bool includeRetweets, unsigned int tweetCount, std::string userInfo, bool isUserId )
{
    /* Prepare URL */
    std::string buildUrl;

    utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_USERTIMELINE_URL +
                        twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                        userInfo, isUserId );
	buildUrl+=twitCurlDefaults::TWITCURL_URL_SEP_AMP+twitCurlDefaults::TWITCURL_INCLUDEENTITIES;
    if( tweetCount )
    {
        if( tweetCount > twitCurlDefaults::MAX_TIMELINE_TWEET_COUNT )
        {
            tweetCount = twitCurlDefaults::MAX_TIMELINE_TWEET_COUNT;
        }
        std::stringstream tmpStrm;
        tmpStrm << twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_COUNT << tweetCount;
        buildUrl += tmpStrm.str();
        tmpStrm.str().clear();
    }

    if( includeRetweets )
    {
        buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_INCRETWEETS;
    }

    if( trimUser )
    {
        buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_AMP + twitCurlDefaults::TWITCURL_TRIMUSER;
    }

    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::userGet
*
* @description: method to get a user's profile
*
* @input: userInfo - screen name or user id in string format,
*         isUserId - true if userInfo contains an id
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::userGet( std::string& userInfo, bool isUserId )
{
    std::string retVal = "";
    if( userInfo.length() )
    {
        /* Set URL */
        std::string buildUrl;
        utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_SHOWUSERS_URL +
                            twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                            userInfo, isUserId );

        /* Perform GET */
        retVal = performGet( buildUrl );
    }
    return retVal;
}

/*++
* @method: twitCurl::friendsGet
*
* @description: method to get a user's friends
*
* @input: userInfo - screen name or user id in string format,
*         isUserId - true if userInfo contains an id
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::friendsGet( std::string userInfo, bool isUserId )
{
    /* Set URL */
    std::string buildUrl;
    utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_SHOWFRIENDS_URL +
                        twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                        userInfo, isUserId );

    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::followersGet
*
* @description: method to get a user's followers
*
* @input: userInfo - screen name or user id in string format,
*         isUserId - true if userInfo contains an id
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::followersGet( std::string userInfo, bool isUserId )
{
    /* Prepare URL */
    std::string buildUrl;
    utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_SHOWFOLLOWERS_URL +
                        twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                        userInfo, isUserId );

    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::directMessageGet
*
* @description: method to get direct messages
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::directMessageGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_DIRECTMESSAGES_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+twitCurlDefaults::TWITCURL_URL_SEP_QUES+twitCurlDefaults::TWITCURL_INCLUDEENTITIES );
}

/*++
* @method: twitCurl::directMessageSend
*
* @description: method to send direct message to a user
*
* @input: userInfo - screen name or user id of a user to whom message needs to be sent,
*         dMsg - message
*         isUserId - true if userInfo contains target user's id
*
* @output: true if POST is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::directMessageSend( std::string& userInfo, std::string& dMsg, bool isUserId )
{
    std::string retVal = "";
    if( userInfo.length() && dMsg.length() )
    {
        /* Prepare new direct message */
        std::string newDm = twitCurlDefaults::TWITCURL_TEXTSTRING + urlencode( dMsg );

        /* Prepare URL */
        std::string buildUrl;
        utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_DIRECTMESSAGENEW_URL +
                            twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                            userInfo, isUserId );

        /* Perform POST */
        retVal = performPost( buildUrl, newDm );
    }
    return retVal;
}

/*++
* @method: twitCurl::directMessageGetSent
*
* @description: method to get sent direct messages
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::directMessageGetSent()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_DIRECTMESSAGESSENT_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::directMessageDestroyById
*
* @description: method to delete direct messages by its id
*
* @input: dMsgId - id of direct message in string format
*
* @output: true if DELETE is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::directMessageDestroyById( std::string& dMsgId )
{
    std::string retVal = "";
    if( dMsgId.length() )
    {
        /* Prepare URL */
        std::string buildUrl = twitterDefaults::TWITCURL_DIRECTMESSAGEDESTROY_URL + dMsgId +
                               twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

        /* Perform DELETE */
        retVal = performDelete( buildUrl );
    }
    return retVal;
}

/*++
* @method: twitCurl::friendshipCreate
*
* @description: method to add a twitter user as friend (follow a user)
*
* @input: userInfo - user id or screen name of a user
*         isUserId - true if userInfo contains a user id instead of screen name
*
* @output: true if POST is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::friendshipCreate( std::string& userInfo, bool isUserId )
{
    std::string retVal = "";
    if( userInfo.length() )
    {
        /* Prepare URL */
        std::string buildUrl;
        utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_FRIENDSHIPSCREATE_URL +
                            twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                            userInfo, isUserId );

        /* Send some dummy data in POST */
        std::string dummyData = twitCurlDefaults::TWITCURL_TEXTSTRING +
                                urlencode( std::string( "dummy" ) );

        /* Perform POST */
        retVal = performPost( buildUrl, dummyData );
    }
    return retVal;
}

/*++
* @method: twitCurl::friendshipDestroy
*
* @description: method to delete a twitter user from friend list (unfollow a user)
*
* @input: userInfo - user id or screen name of a user
*         isUserId - true if userInfo contains a user id instead of screen name
*
* @output: true if DELETE is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::friendshipDestroy( std::string& userInfo, bool isUserId )
{
    std::string retVal = "";
    if( userInfo.length() )
    {
        /* Prepare URL */
        std::string buildUrl;
        utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_FRIENDSHIPSDESTROY_URL +
                            twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                            userInfo, isUserId );

        /* Perform DELETE */
        retVal = performDelete( buildUrl );
    }
    return retVal;
}

/*++
* @method: twitCurl::friendshipShow
*
* @description: method to show all friends
*
* @input: userInfo - user id or screen name of a user of whom friends need to be shown
*         isUserId - true if userInfo contains a user id instead of screen name
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::friendshipShow( std::string& userInfo, bool isUserId )
{
    /* Prepare URL */
    std::string buildUrl = twitterDefaults::TWITCURL_FRIENDSHIPSSHOW_URL +
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];
    if( userInfo.length() )
    {
        /* Append username to the URL */
        buildUrl += twitCurlDefaults::TWITCURL_URL_SEP_QUES;
        if( isUserId )
        {
            buildUrl += twitCurlDefaults::TWITCURL_TARGETUSERID;
        }
        else
        {
            buildUrl += twitCurlDefaults::TWITCURL_TARGETSCREENNAME;
        }
        buildUrl += userInfo;
    }

    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::friendsIdsGet
*
* @description: method to show IDs of all friends of a twitter user
*
* @input: userInfo - user id or screen name of a user
*         isUserId - true if userInfo contains a user id instead of screen name
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::friendsIdsGet( std::string& userInfo, bool isUserId )
{
    /* Prepare URL */
    std::string buildUrl;
    utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_FRIENDSIDS_URL +
                        twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                        userInfo, isUserId );

    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::followersIdsGet
*
* @description: method to show IDs of all followers of a twitter user
*
* @input: userInfo - user id or screen name of a user
*         isUserId - true if userInfo contains a user id instead of screen name
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::followersIdsGet( std::string& userInfo, bool isUserId )
{
    /* Prepare URL */
    std::string buildUrl;
    utilMakeUrlForUser( buildUrl, twitterDefaults::TWITCURL_FOLLOWERSIDS_URL +
                        twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType],
                        userInfo, isUserId );
	buildUrl+="&stringify_ids=true";
    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::accountRateLimitGet
*
* @description: method to get API rate limit of current user
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::accountRateLimitGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_ACCOUNTRATELIMIT_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::favoriteGet
*
* @description: method to get favorite users' statuses
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::favoriteGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_FAVORITESGET_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType]+twitCurlDefaults::TWITCURL_URL_SEP_QUES+twitCurlDefaults::TWITCURL_INCLUDEENTITIES );
}

/*++
* @method: twitCurl::favoriteCreate
*
* @description: method to favorite a status message
*
* @input: statusId - id in string format of the status to be favorited
*
* @output: true if POST is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::favoriteCreate( std::string& statusId )
{
	/* Prepare URL */
	std::string buildUrl = twitterDefaults::TWITCURL_FAVORITECREATE_URL + 
		twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

	/* Perform DELETE */
	return performPost( buildUrl ,"id="+statusId);
}

/*++
* @method: twitCurl::favoriteDestroy
*
* @description: method to delete a favorited the status
*
* @input: statusId - id in string format of the favorite status to be deleted
*
* @output: true if DELETE is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::favoriteDestroy( std::string& statusId )
{
    /* Prepare URL */
    std::string buildUrl = twitterDefaults::TWITCURL_FAVORITEDESTROY_URL + 
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

    /* Perform DELETE */
    return performPost( buildUrl ,"id="+statusId);
}
std::string twitCurl::retweetCreate( std::string& statusId )
{
	/* Prepare URL */
	std::string buildUrl = twitterDefaults::TWITCURL_RETWEETCREATE_URL + statusId +
		twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

	/* Send some dummy data in POST */
	std::string dummyData = twitCurlDefaults::TWITCURL_TEXTSTRING +
		urlencode( std::string( "dummy" ) );

	/* Perform POST */
	return performPost( buildUrl, dummyData );
}

/*++
* @method: twitCurl::blockCreate
*
* @description: method to block a user
*
* @input: userInfo - user id or screen name
*
* @output: true if POST is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::blockCreate( std::string& userInfo )
{
        /* Prepare URL */
        std::string buildUrl = twitterDefaults::TWITCURL_BLOCKSCREATE_URL + userInfo +
                               twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

        /* Send some dummy data in POST */
        std::string dummyData = twitCurlDefaults::TWITCURL_TEXTSTRING +
                                urlencode( std::string( "dummy" ) );

        /* Perform POST */
        return performPost( buildUrl, dummyData );
}

/*++
* @method: twitCurl::blockDestroy
*
* @description: method to unblock a user
*
* @input: userInfo - user id or screen name
*
* @output: true if DELETE is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::blockDestroy( std::string& userInfo )
{
    /* Prepare URL */
    std::string buildUrl = twitterDefaults::TWITCURL_BLOCKSDESTROY_URL + userInfo +
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

    /* Perform DELETE */
    return performDelete( buildUrl );
}

/*++
* @method: twitCurl::savedSearchGet
*
* @description: gets authenticated user's saved search queries.
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::savedSearchGet( )
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_SAVEDSEARCHGET_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::savedSearchShow
*
* @description: method to retrieve the data for a saved search owned by the authenticating user 
*               specified by the given id.
*
* @input: searchId - id in string format of the search to be displayed
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::savedSearchShow( std::string& searchId )
{
    /* Prepare URL */
    std::string buildUrl = twitterDefaults::TWITCURL_SAVEDSEARCHSHOW_URL + searchId +
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

    /* Perform GET */
    return performGet( buildUrl );
}

/*++
* @method: twitCurl::savedSearchCreate
*
* @description: creates a saved search for the authenticated user
*
* @input: query - the query of the search the user would like to save
*
* @output: true if POST is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::savedSearchCreate( std::string& query )
{
    /* Prepare URL */
    std::string buildUrl = twitterDefaults::TWITCURL_SAVEDSEARCHCREATE_URL +
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

    /* Send some dummy data in POST */
    std::string queryStr = twitCurlDefaults::TWITCURL_QUERYSTRING + urlencode( query );

    /* Perform POST */
    return performPost( buildUrl, queryStr );
}


/*++
* @method: twitCurl::savedSearchDestroy
*
* @description: method to destroy a saved search for the authenticated user. The search specified 
*               by id must be owned by the authenticating user.
*
* @input: searchId - search id of item to be deleted
*
* @output: true if DELETE is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::savedSearchDestroy( std::string& searchId )
{
    /* Prepare URL */
    std::string buildUrl = twitterDefaults::TWITCURL_SAVEDSEARCHDESTROY_URL + searchId +
                           twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType];

    /* Perform DELETE */
    return performDelete( buildUrl );
}


/*++
* @method: twitCurl::trendsGet()
*
* @description: gets trends.
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::trendsGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_TRENDS_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}


/*++
* @method: twitCurl::trendsDailyGet()
*
* @description: gets daily trends.
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::trendsDailyGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_TRENDSDAILY_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::trendsWeeklyGet()
*
* @description: gets weekly trends.
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::trendsWeeklyGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_TRENDSWEEKLY_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::trendsCurrentGet()
*
* @description: gets current trends.
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::trendsCurrentGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_TRENDSCURRENT_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::trendsAvailableGet()
*
* @description: gets available trends.
*
* @input: none
*
* @output: true if GET is success, otherwise false. This does not check http
*          response by twitter. Use getLastWebResponse() for that.
*
*--*/
std::string twitCurl::trendsAvailableGet()
{
    /* Perform GET */
    return performGet( twitterDefaults::TWITCURL_TRENDSAVAILABLE_URL +
                       twitCurlDefaults::TWITCURL_EXTENSIONFORMATS[m_eApiFormatType] );
}

/*++
* @method: twitCurl::curlCallback
*
* @description: static method to get http response back from cURL.
*               this is an internal method, users of twitcurl need not
*               use this.
*
* @input: as per cURL convention.
*
* @output: size of data stored in our buffer
*
* @remarks: internal method
*
*--*/
int twitCurl::curlCallback( char* data, size_t size, size_t nmemb, twitInstance *instance )
{
	twitCurl* pTwitCurlObj=instance->twit;
    int writtenSize = 0;
    if( ( NULL != pTwitCurlObj ) && ( NULL != data ) )
    {
        /* Save http response in twitcurl object's buffer */
        writtenSize = pTwitCurlObj->saveLastWebResponse( data, ( size*nmemb ) ,instance);
    }
    return writtenSize;
}

/*++
* @method: twitCurl::saveLastWebResponse
*
* @description: method to save http responses. this is an internal method
*               and twitcurl users need not use this.
*
* @input: data - character buffer from cURL,
*         size - size of character buffer
*
* @output: size of data stored in our buffer
*
* @remarks: internal method
*
*--*/
int twitCurl::saveLastWebResponse(  char*& data, size_t size, twitInstance *instance )
{
    int bytesWritten = 0;
    if( data && size )
    {
        /* Append data in our internal buffer */
        instance->data.append( data, size );
        bytesWritten = (int)size; 
    }
    return bytesWritten;
}

/*++
* @method: twitCurl::clearCurlCallbackBuffers
*
* @description: method to clear callback buffers used by cURL. this is an
*               internal method and twitcurl users need not use this.
*
* @input: none
*
* @output: none
*
* @remarks: internal method
*
*--*/
void twitCurl::clearCurlCallbackBuffers()
{
    memset( m_errorBuffer, 0, twitCurlDefaults::TWITCURL_DEFAULT_BUFFSIZE );
}

/*++
* @method: twitCurl::prepareCurlProxy
*
* @description: method to set proxy details into cURL. this is an internal method.
*               twitcurl users should not use this method, instead use setProxyXxx
*               methods to set proxy server information.
*
* @input: none
*
* @output: none
*
* @remarks: internal method
*
*--*/
void twitCurl::prepareCurlProxy(twitInstance *instance)
{
    //if( !m_curlProxyParamsSet)
    {

		curl_easy_setopt( instance->m_curlHandle, CURLOPT_CAINFO,"resources/cacert.pem");
		curl_easy_setopt( instance->m_curlHandle, CURLOPT_PROXY, NULL );
		curl_easy_setopt( instance->m_curlHandle, CURLOPT_PROXYUSERPWD, NULL );
		curl_easy_setopt( instance->m_curlHandle, CURLOPT_PROXYAUTH, (long)CURLAUTH_ANY );
		if(getProxyServerIp().empty())
			return;
        /* Reset existing proxy details in cURL */

        /* Set proxy details in cURL */
        std::string proxyIpPort;
        utilMakeCurlParams( proxyIpPort, getProxyServerIp(), getProxyServerPort() );
        curl_easy_setopt( instance->m_curlHandle, CURLOPT_PROXY, proxyIpPort.c_str() );
		curl_easy_setopt(instance->m_curlHandle, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);

		//curl_easy_setopt( instance->m_curlHandle, CURLOPT_CAINFO,"cacert.pem");
        /* Prepare username and password for proxy server */
        if( m_proxyUserName.length() && m_proxyPassword.length() )
        {
            std::string proxyUserPass;
            utilMakeCurlParams( proxyUserPass, getProxyUserName(), getProxyPassword() );
            curl_easy_setopt( instance->m_curlHandle, CURLOPT_PROXYUSERPWD, proxyUserPass.c_str() );
        }

        /* Set the flag to true indicating that proxy info is set in cURL */
    }
}

/*++
* @method: twitCurl::prepareCurlCallback
*
* @description: method to set callback details into cURL. this is an internal method.
*               twitcurl users should not use this method.
*
* @input: none
*
* @output: none
*
* @remarks: internal method
*
*--*/
void twitCurl::prepareCurlCallback(twitInstance *instance)
{
   // if( !m_curlCallbackParamsSet )
    {
        /* Set buffer to get error */
        curl_easy_setopt( instance->m_curlHandle, CURLOPT_ERRORBUFFER, m_errorBuffer );

        /* Set callback function to get response */
        curl_easy_setopt( instance->m_curlHandle, CURLOPT_WRITEFUNCTION, curlCallback );
        curl_easy_setopt( instance->m_curlHandle, CURLOPT_WRITEDATA, instance );

        /* Set the flag to true indicating that callback info is set in cURL */
    }
}

/*++
* @method: twitCurl::prepareCurlUserPass
*
* @description: method to set twitter credentials into cURL. this is an internal method.
*               twitcurl users should not use this method, instead use setTwitterXxx
*               methods to set twitter username and password.
*
* @input: none
*
* @output: none
*
* @remarks: internal method
*
*--*/
void twitCurl::prepareCurlUserPass(twitInstance *instance)
{
   // if( !m_curlLoginParamsSet  || 1)
    {
        /* Reset existing username and password stored in cURL */
        curl_easy_setopt( instance->m_curlHandle, CURLOPT_USERPWD, NULL );

        /* Prepare username:password */
        std::string userNamePassword;
        utilMakeCurlParams( userNamePassword, getTwitterUsername(), getTwitterPassword() );

        /* Set username and password */
        curl_easy_setopt( instance->m_curlHandle, CURLOPT_USERPWD, userNamePassword.c_str() );

        /* Set the flag to true indicating that twitter credentials are set in cURL */
//        m_curlLoginParamsSet = true;
    }
}

/*++
* @method: twitCurl::prepareStandardParams
*
* @description: method to set standard params into cURL. this is an internal method.
*               twitcurl users should not use this method.
*
* @input: none
*
* @output: none
*
* @remarks: internal method
*
*--*/
int curl_debug_callbacktwit(CURL *curl,curl_infotype infotype,char *data,size_t size,void *userptr)
{
printf("%i:\n");
fwrite(data,size,1,stdout);
printf("\n");
FILE *fp=fopen("stream debug2.txt","a+");
fprintf(fp,"%i:\n");
fwrite(data,size,1,fp);
fprintf(fp,"\n\n\n");
fclose(fp);
return 0;
}
void twitCurl::prepareStandardParams(twitInstance *instance)
{
    /* Restore any custom request we may have */
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_CUSTOMREQUEST, NULL );
	//curl_easy_setopt(instance->m_curlHandle,CURLOPT_VERBOSE,1);
	//curl_easy_setopt( instance->m_curlHandle, CURLOPT_DEBUGFUNCTION, curl_debug_callbacktwit);
    /* Clear callback and error buffers */
    clearCurlCallbackBuffers();

    /* Prepare proxy */
    prepareCurlProxy(instance);

    /* Prepare cURL callback data and error buffer */
    prepareCurlCallback(instance);

    /* Prepare username and password for twitter */
    prepareCurlUserPass(instance);
}

/*++
* @method: twitCurl::performGet
*
* @description: method to send http GET request. this is an internal method.
*               twitcurl users should not use this method.
*
* @input: getUrl - url
*
* @output: none
*
* @remarks: internal method
*
*--*/
std::string twitCurl::performGet( const std::string& getUrl )
{
    std::string dataStrDummy;
    std::string oAuthHttpHeader;
    struct curl_slist* pOAuthHeaderList = NULL;

    /* Prepare standard params */
	twitInstance *instance=new twitInstance;
	instance->instance=time(NULL)+rand();
	instance->twit=this;
	instance->m_curlHandle=curl_easy_init();
    prepareStandardParams(instance);

    /* Set OAuth header */
    m_oAuth.getOAuthHeader( eOAuthHttpGet, getUrl, dataStrDummy, oAuthHttpHeader );
    if( oAuthHttpHeader.length() )
    {
        pOAuthHeaderList = curl_slist_append( pOAuthHeaderList, oAuthHttpHeader.c_str() );
        if( pOAuthHeaderList )
        {
            curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPHEADER, pOAuthHeaderList );
        }
    }

    /* Set http request and url */
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPGET, 1 );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_URL, getUrl.c_str() );

    /* Send http request */
    if( CURLE_OK == curl_easy_perform( instance->m_curlHandle ) )
    {
        if( pOAuthHeaderList )
        {
            curl_slist_free_all( pOAuthHeaderList );
        }
		/*if(m_callbackData.size()==0 || m_callbackData[0]=='<')//assume error)
		{
			time_t t=time(NULL);
			while(time(NULL)-t<5);
			curl_easy_cleanup(instance->m_curlHandle); delete instance;
			return performGet(getUrl);
		}*/
		std::string ret=instance->data;
		
		curl_easy_cleanup(instance->m_curlHandle); delete instance;
        return ret;
    }
    if( pOAuthHeaderList )
    {
        curl_slist_free_all( pOAuthHeaderList );
	}
	curl_easy_cleanup(instance->m_curlHandle); delete instance;
    return "";
}

/*++
* @method: twitCurl::performGet
*
* @description: method to send http GET request. this is an internal method.
*               twitcurl users should not use this method.
*
* @input: const std::string& getUrl, const std::string& oAuthHttpHeader
*
* @output: none
*
* @remarks: internal method
*
*--*/
std::string twitCurl::performGet( const std::string& getUrl, const std::string& oAuthHttpHeader )
{
    struct curl_slist* pOAuthHeaderList = NULL;

	/* Prepare standard params */
	twitInstance *instance=new twitInstance;
	instance->instance=time(NULL)+rand();
	instance->twit=this;
	instance->m_curlHandle=curl_easy_init();
	prepareStandardParams(instance);

    /* Set http request and url */
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPGET, 1 );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_URL, getUrl.c_str() );

    /* Set header */
    if( oAuthHttpHeader.length() )
    {
        pOAuthHeaderList = curl_slist_append( pOAuthHeaderList, oAuthHttpHeader.c_str() );
        if( pOAuthHeaderList )
        {
            curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPHEADER, pOAuthHeaderList );
        }
    }

    /* Send http request */
    if( CURLE_OK == curl_easy_perform( instance->m_curlHandle ) )
    {
        if( pOAuthHeaderList )
        {
            curl_slist_free_all( pOAuthHeaderList );
		}
		std::string ret=instance->data;
		curl_easy_cleanup(instance->m_curlHandle); delete instance;
		return ret;
    }
    if( pOAuthHeaderList )
    {
        curl_slist_free_all( pOAuthHeaderList );
	}
	curl_easy_cleanup(instance->m_curlHandle); delete instance;
    return "";
}

/*++
* @method: twitCurl::performDelete
*
* @description: method to send http DELETE request. this is an internal method.
*               twitcurl users should not use this method.
*
* @input: deleteUrl - url
*
* @output: none
*
* @remarks: internal method
*
*--*/
std::string twitCurl::performDelete( const std::string& deleteUrl )
{

    std::string dataStrDummy;
    std::string oAuthHttpHeader;
    struct curl_slist* pOAuthHeaderList = NULL;

	/* Prepare standard params */
	twitInstance *instance=new twitInstance;
	instance->instance=time(NULL)+rand();
	instance->twit=this;
	instance->m_curlHandle=curl_easy_init();
	prepareStandardParams(instance);

    /* Set OAuth header */
    m_oAuth.getOAuthHeader( eOAuthHttpDelete, deleteUrl, dataStrDummy, oAuthHttpHeader );
    if( oAuthHttpHeader.length() )
    {
        pOAuthHeaderList = curl_slist_append( pOAuthHeaderList, oAuthHttpHeader.c_str() );
        if( pOAuthHeaderList )
        {
            curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPHEADER, pOAuthHeaderList );
        }
    }

    /* Set http request and url */
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE" );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_URL, deleteUrl.c_str() );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_COPYPOSTFIELDS, dataStrDummy.c_str() );

    /* Send http request */
    if( CURLE_OK == curl_easy_perform( instance->m_curlHandle ) )
    {
        if( pOAuthHeaderList )
        {
            curl_slist_free_all( pOAuthHeaderList );
		}
		std::string ret=instance->data;
		curl_easy_cleanup(instance->m_curlHandle); delete instance;
		return ret;
    }
    if( pOAuthHeaderList )
    {
        curl_slist_free_all( pOAuthHeaderList );
	}
	curl_easy_cleanup(instance->m_curlHandle); delete instance;
    return "";
}

/*++
* @method: twitCurl::performPost
*
* @description: method to send http POST request. this is an internal method.
*               twitcurl users should not use this method.
*
* @input: postUrl - url,
*         dataStr - url encoded data to be posted
*
* @output: none
*
* @remarks: internal method
*           data value in dataStr must already be url encoded.
*           ex: dataStr = "key=urlencode(value)"
*
*--*/
std::string twitCurl::performPost( const std::string& postUrl, std::string dataStr )
{

    std::string oAuthHttpHeader;
    struct curl_slist* pOAuthHeaderList = NULL;

	/* Prepare standard params */
	twitInstance *instance=new twitInstance;
	instance->instance=time(NULL)+rand();
	instance->twit=this;
	instance->m_curlHandle=curl_easy_init();
	prepareStandardParams(instance);

    /* Set OAuth header */
    m_oAuth.getOAuthHeader( eOAuthHttpPost, postUrl, dataStr, oAuthHttpHeader );
    if( oAuthHttpHeader.length() )
    {
        pOAuthHeaderList = curl_slist_append( pOAuthHeaderList, oAuthHttpHeader.c_str() );
        if( pOAuthHeaderList )
        {
            curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPHEADER, pOAuthHeaderList );
        }
    }

    /* Set http request, url and data */
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_POST, 1 );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_URL, postUrl.c_str() );
    if( dataStr.length() )
    {
        curl_easy_setopt( instance->m_curlHandle, CURLOPT_COPYPOSTFIELDS, dataStr.c_str() );
    }

    /* Send http request */
    if( CURLE_OK == curl_easy_perform( instance->m_curlHandle ) )
    {
        if( pOAuthHeaderList )
        {
            curl_slist_free_all( pOAuthHeaderList );
		}
		std::string ret=instance->data;
		curl_easy_cleanup(instance->m_curlHandle); delete instance;
		return ret;
    }
    if( pOAuthHeaderList )
    {
        curl_slist_free_all( pOAuthHeaderList );
	}
	curl_easy_cleanup(instance->m_curlHandle); delete instance;
    return "";
}

/*++
* @method: utilMakeCurlParams
*
* @description: utility function to build parameter strings in the format
*               required by cURL ("param1:param2"). twitcurl users should
*               not use this function.
*
* @input: inParam1 - first parameter,
*         inParam2 - second parameter
*
* @output: outStr - built parameter
*
* @remarks: internal method
*
*--*/
void utilMakeCurlParams( std::string& outStr, std::string& inParam1, std::string& inParam2 )
{
    outStr = inParam1;
    outStr += twitCurlDefaults::TWITCURL_COLON + inParam2;
}

/*++
* @method: utilMakeUrlForUser
*
* @description: utility function to build url compatible to twitter. twitcurl
*               users should not use this function.
*
* @input: baseUrl - base twitter url,
*         userInfo - user name,
*         isUserId - indicates if userInfo contains a user id or scree name
*
* @output: outUrl - built url
*
* @remarks: internal method
*
*--*/
void utilMakeUrlForUser( std::string& outUrl, const std::string& baseUrl, std::string& userInfo, bool isUserId )
{
    /* Copy base URL */
    outUrl = baseUrl;

    if( userInfo.length() )
    {
        /* Append username to the URL */
        outUrl += twitCurlDefaults::TWITCURL_URL_SEP_QUES;
        if( isUserId )
        {
            outUrl += twitCurlDefaults::TWITCURL_USERID;
        }
        else
        {
            outUrl += twitCurlDefaults::TWITCURL_SCREENNAME;
        }
        outUrl += userInfo;
    }
}

/*++
* @method: twitCurl::getOAuth
*
* @description: method to get a reference to oAuth object.
*
* @input: none
*
* @output: reference to oAuth object
*
*--*/
oAuth& twitCurl::getOAuth()
{
    return m_oAuth;
}

/*++
* @method: twitCurl::oAuthRequestToken
*
* @description: method to get a request token key and secret. this token
*               will be used to get authorize user and get PIN from twitter
*
* @input: authorizeUrl is an output parameter. this method will set the url
*         in this string. user should visit this link and get PIN from that page.
*
* @output: true if everything went sucessfully, otherwise false
*
*--*/
std::string twitCurl::oAuthRequestToken( std::string& authorizeUrl /* out */ )
{

    /* Get OAuth header for request token */
    std::string retVal = "";
    std::string oAuthHeader;
    authorizeUrl = "";
    if( m_oAuth.getOAuthHeader( eOAuthHttpGet,
                                oAuthTwitterApiUrls::OAUTHLIB_TWITTER_REQUEST_TOKEN_URL,
                                std::string( "" ),
                                oAuthHeader ) )
    {
		std::string twitterResp=performGet( oAuthTwitterApiUrls::OAUTHLIB_TWITTER_REQUEST_TOKEN_URL, oAuthHeader );
        if( twitterResp!="" )
        {
            /* Tell OAuth object to save access token and secret from web response */
            
            m_oAuth.extractOAuthTokenKeySecret( twitterResp );

            /* Get access token and secret from OAuth object */
            std::string oAuthTokenKey;
            m_oAuth.getOAuthTokenKey( oAuthTokenKey );

            /* Build authorize url so that user can visit in browser and get PIN */
            authorizeUrl.assign( oAuthTwitterApiUrls::OAUTHLIB_TWITTER_AUTHORIZE_URL );
            authorizeUrl.append( oAuthTokenKey.c_str() );

            retVal = "SUCCESS";
        }
    }
    return retVal;
}

/*++
* @method: twitCurl::oAuthAccessToken
*
* @description: method to exchange request token with access token
*
* @input: none
*
* @output: true if everything went sucessfully, otherwise false
*
*--*/
std::string twitCurl::oAuthAccessToken()
{
    /* Get OAuth header for access token */
    std::string retVal = "";
    std::string oAuthHeader;
    if( m_oAuth.getOAuthHeader( eOAuthHttpGet,
                                oAuthTwitterApiUrls::OAUTHLIB_TWITTER_ACCESS_TOKEN_URL,
                                std::string( "" ),
                                oAuthHeader, true ) )
	{
		std::string twitterResp=performGet( oAuthTwitterApiUrls::OAUTHLIB_TWITTER_ACCESS_TOKEN_URL, oAuthHeader );
        if( twitterResp !="")
        {
            /* Tell OAuth object to save access token and secret from web response */
            m_oAuth.extractOAuthTokenKeySecret( twitterResp );

            retVal = "SUCCESS";
        }
    }
    return retVal;
}

/*++ 
* ADDED BY ANTIROOT 
*
* @method: twitCurl::oAuthHandlePIN
*
* @description: method to handle user's PIN code from the authentiation URLs 
*
* @input: none
*
* @output: true if everything went sucessfully, otherwise false
*
*--*/
std::string twitCurl::oAuthHandlePIN( const std::string& authorizeUrl /* in */ )
{

    std::string dataStr;
    std::string oAuthHttpHeader;
    std::string authenticityTokenVal;
    std::string oauthTokenVal;
    std::string pinCodeVal;
    unsigned long httpStatusCode = 0;
    size_t nPosStart, nPosEnd;
    struct curl_slist* pOAuthHeaderList = NULL;

	/* Prepare standard params */
	twitInstance *instance=new twitInstance;
	instance->instance=time(NULL)+rand();
	instance->twit=this;
	int n=0;while(handleInUse){printf("%i: %i\n",instance->instance,n++);}instance->m_curlHandle=m_curlHandle;handleInUse=1;
	prepareStandardParams(instance);

    /* Set OAuth header */
    m_oAuth.getOAuthHeader( eOAuthHttpGet, authorizeUrl, dataStr, oAuthHttpHeader );
    if( oAuthHttpHeader.length() )
    {
        pOAuthHeaderList = curl_slist_append( pOAuthHeaderList, oAuthHttpHeader.c_str() );
        if( pOAuthHeaderList )
        {
            curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPHEADER, pOAuthHeaderList );
        }
    }

    /* Set http request and url */
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPGET, 1 );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_URL, authorizeUrl.c_str() );

    /* Send http request */
    if( CURLE_OK == curl_easy_perform( instance->m_curlHandle ) )
    {
        if( pOAuthHeaderList )
        {
            curl_easy_getinfo( instance->m_curlHandle, CURLINFO_HTTP_CODE, &httpStatusCode );
            curl_slist_free_all( pOAuthHeaderList );

            // Now, let's find the authenticity token and oauth token 
            nPosStart = instance->data.find( oAuthLibDefaults::OAUTHLIB_AUTHENTICITY_TOKEN_TWITTER_RESP_KEY );
            if( std::string::npos == nPosStart )
            {
				curl_easy_cleanup(instance->m_curlHandle); delete instance;
                return "";
            }
            nPosStart += oAuthLibDefaults::OAUTHLIB_AUTHENTICITY_TOKEN_TWITTER_RESP_KEY.length();
            nPosEnd = instance->data.substr( nPosStart ).find( oAuthLibDefaults::OAUTHLIB_TOKEN_END_TAG_TWITTER_RESP );
            if( std::string::npos == nPosEnd )
			{
				curl_easy_cleanup(instance->m_curlHandle); delete instance;
                return "";
            }
            authenticityTokenVal = instance->data.substr( nPosStart, nPosEnd );

            nPosStart = instance->data.find( oAuthLibDefaults::OAUTHLIB_TOKEN_TWITTER_RESP_KEY );
            if( std::string::npos == nPosStart )
			{
				curl_easy_cleanup(instance->m_curlHandle); delete instance;
                return "";
            }
            nPosStart += oAuthLibDefaults::OAUTHLIB_TOKEN_TWITTER_RESP_KEY.length();
            nPosEnd = instance->data.substr( nPosStart ).find( oAuthLibDefaults::OAUTHLIB_TOKEN_END_TAG_TWITTER_RESP );
            if( std::string::npos == nPosEnd )
			{
				curl_easy_cleanup(instance->m_curlHandle); delete instance;
                return "";
            }
            oauthTokenVal = instance->data.substr( nPosStart, nPosEnd );
        }
    } 
    else if( pOAuthHeaderList )
    {
		curl_slist_free_all( pOAuthHeaderList );
		curl_easy_cleanup(instance->m_curlHandle); delete instance;
        return "";
    }

    // Second phase for the authorization 
    pOAuthHeaderList = NULL;
    oAuthHttpHeader.clear();

    /* Prepare standard params */
    prepareStandardParams(instance);

    /*
    Now, we need to make a data string for POST operation
    which includes oauth token, authenticity token, username, password.
    */
    dataStr = oAuthLibDefaults::OAUTHLIB_TOKEN_KEY + "=" + oauthTokenVal + "&" +                      \
              oAuthLibDefaults::OAUTHLIB_AUTHENTICITY_TOKEN_KEY + "=" + authenticityTokenVal + "&" +  \
              oAuthLibDefaults::OAUTHLIB_SESSIONUSERNAME_KEY + "=" + getTwitterUsername() + "&" +     \
              oAuthLibDefaults::OAUTHLIB_SESSIONPASSWORD_KEY + "=" + getTwitterPassword();

    /* Set OAuth header */
    m_oAuth.getOAuthHeader( eOAuthHttpPost, authorizeUrl, dataStr, oAuthHttpHeader );
    if( oAuthHttpHeader.length() )
    {
        pOAuthHeaderList = curl_slist_append( pOAuthHeaderList, oAuthHttpHeader.c_str() );
        if( pOAuthHeaderList )
        {
            curl_easy_setopt( instance->m_curlHandle, CURLOPT_HTTPHEADER, pOAuthHeaderList );
        }
    }

    /* Set http request and url */
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_POST, 1 );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_URL, authorizeUrl.c_str() );
    curl_easy_setopt( instance->m_curlHandle, CURLOPT_COPYPOSTFIELDS, dataStr.c_str() );

    /* Send http request */
    if( CURLE_OK == curl_easy_perform( instance->m_curlHandle ) )
    {
        if( pOAuthHeaderList )
        {
            curl_easy_getinfo( instance->m_curlHandle, CURLINFO_HTTP_CODE, &httpStatusCode );
            curl_slist_free_all( pOAuthHeaderList );

            // Now, let's find the PIN CODE  
            nPosStart = instance->data.find( oAuthLibDefaults::OAUTHLIB_PIN_TWITTER_RESP_KEY );
            if( std::string::npos == nPosStart )
			{
				curl_easy_cleanup(instance->m_curlHandle); delete instance;
                return "";
            }
            nPosStart += oAuthLibDefaults::OAUTHLIB_PIN_TWITTER_RESP_KEY.length();
            nPosEnd = instance->data.substr( nPosStart ).find( oAuthLibDefaults::OAUTHLIB_PIN_END_TAG_TWITTER_RESP );
            if( std::string::npos == nPosEnd )
			{
				curl_easy_cleanup(instance->m_curlHandle); delete instance;
                return "";
            }
            pinCodeVal = instance->data.substr( nPosStart, nPosEnd );
            getOAuth().setOAuthPin( pinCodeVal );
			std::string ret=instance->data;
			curl_easy_cleanup(instance->m_curlHandle); delete instance;
            return ret;
        }
    } 
    else if( pOAuthHeaderList )
    {
        curl_slist_free_all( pOAuthHeaderList );
	}
	curl_easy_cleanup(instance->m_curlHandle); delete instance;
    return "";
}
