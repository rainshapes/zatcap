function init()
{updateTweetLength();
	document.getElementById('tweetbox').onkeydown=function(e)
	{
		if(e.which == 13 && e.shiftKey) 
		{
			
		}
		else if (e.which == 13) 
		{
		   e.preventDefault(); //Stops enter from creating a new line
		   sendTweet();
		}
	};
}
var inReplyTo="";
function moveCaretToEnd(el) {
    if (typeof el.selectionStart == "number") {
        el.selectionStart = el.selectionEnd = el.value.length;
    } else if (typeof el.createTextRange != "undefined") {
        el.focus();
        var range = el.createTextRange();
        range.collapse(false);
        range.select();
    }
}
function setReplyTo(id,author)
{
	var text=document.getElementById(id+'_text').innerHTML;
	inReplyTo=id;
	var to=new Array();
	if(!globals || author!='@'+globals.username)
		to.push(author);
	for(var i=0;i<text.length;i++)
	{
		if(text.charAt(i)=='@')
		{
			var str='';
			for(var j=i;j<text.length;j++)
			{
				var c=text.charCodeAt(j)
				if((c<48 || (c>57 && c<65) || (c>90 && c<97) || c>122) && c!=95 && c!=64)
					break;
				str=str+text.charAt(j);
			}
			if(globals && str!='@'+globals.username && str.length>0 && to.indexOf(str)==-1)
				to.push(str);
			i=j;
		}
				
	}
	for(var i=0;i<to.length;i++)
		document.getElementById('tweetbox').value+=to[i]+" ";
	document.getElementById('tweetbox').focus();
	moveCaretToEnd(document.getElementById('tweetbox'));
	
}
function sendTweet(tweet)
{
	if(!tweet)
		tweet=document.getElementById('tweetbox').value;
	cpp.sendTweet(tweet,inReplyTo);
	document.getElementById('tweetbox').value="";
}
function updateTweetLength()
{
	document.getElementById('TweetLength').innerHTML=""+(140-document.getElementById('tweetbox').value.length);
}
function nodeFromHtml(html)
{
	var t=document.createElement('span');
	t.innerHTML=html;
	return t;
}
function addTweet(columnName,n)
{
	var a=document.getElementById(columnName).children;
	var tweetId=n.tweetid;
	if(a.length==0)
	{
		document.getElementById(columnName).insertBefore(n,null);
		//cpp.print("Inserted "+tweetId+" at index 0");
		return tweetId;
	}
	//cpp.print("not empty");
	var i;
	for(i=0;i<a.length;i++)
	{
		//cpp.debug("id: " +a[i].id+" ("+tweetId+")");
		if(a[i]!== undefined && tweetId>a[i].tweetid)
		{
			//cpp.print("Inserted "+tweetId+" at index "+i);
			document.getElementById(columnName).insertBefore(n,a[i]);
			return tweetId;
		}
		if(a[i].tweetid===undefined)
			cpp.debug("no id");
	}
	//cpp.print("Inserted "+tweetId+" at index(end) "+i);
			document.getElementById(columnName).insertBefore(n,null);
//	cpp.debug("insert at "+i);
	return tweetId;
}
function addTweetHtml(columnName,tweetHtml,tweetId)
{
	//cpp.print("Adding "+tweetId);
	var column=document.getElementById(columnName);
	var n=nodeFromHtml(tweetHtml);	
	n.tweetid=tweetId;
	if(column.scrollTop<10) 
	{
		addTweet(columnName,n);
		}
	else if(column.pendingTweets===undefined)
			cpp.debug("no pending tweets existing");
			else
			{
		column.pendingTweets.push(n);}
}
function removeTweet(columnName,id)
{
//cpp.debug(id+"_"+columnName);
	remove(id+"_"+columnName);
}
function checkScroll(columnName)
{
	var column=document.getElementById(columnName);
	if(column.scrollTop<10) 
	{
			//cpp.debug("pending tweets: "+column.pendingTweets.length);
		if(column.pendingTweets===undefined)
			cpp.debug("no pending tweets existing");
		else if(column.pendingTweets.length>0)
		{
			var oldFirst=column.firstChild;
			for(var i=0;i<column.pendingTweets.length;i++)
				addTweet(columnName,column.pendingTweets[i]);
			column.scrollTop=oldFirst.offsetTop;
		}
	}
}
function doFavorite(id,columnName)
{
	var favImg=document.getElementById(id+"_"+columnName+"_fav");
	if(favImg.src=="asset://resource/favorite.png" || favImg.src=="asset://resource/favorite_hover.png")
	{
		cpp.favorite(id);
		favImg.src="asset://resource/favorite_on.png";
	}
	else
	{
		cpp.unfavorite(id);
		favImg.src="asset://resource/favorite.png";
	}			
}
function doRetweet(id,columnName)
{
	var favImg=document.getElementById(id+"_"+columnName+"_rt");
	if(favImg.src=="asset://resource/retweet.png" || favImg.src=="asset://resource/retweet_hover.png")
	{
		cpp.retweet(id);
		favImg.src="asset://resource/retweet_on.png";
	}	
}
function doDelete(id,columnName)
{
	var favImg=document.getElementById(id+"_"+columnName+"_del");
	if(favImg.src=="asset://resource/delete.png" || favImg.src=="asset://resource/delete_hover.png")
	{
		cpp._delete(id);
	}	
}
function remove(id)
{
    return (elem=document.getElementById(id)).parentNode.removeChild(elem);
}
function lightbox(url)
{
	document.getElementById('body').insertBefore(nodeFromHtml("<div id=\"light\" class=\"white_content\"><object data='"+url+"' style='width: 95%;height: 95%'>tada</object><center><a href='javascript:;' onclick=\"cpp.openInNativeBrowser('"+url+"');\">Open in browser</a></center></div><div id=\"fade\" class=\"black_overlay\" onclick=\"remove('light');remove('fade');\"></div>"),null);
}