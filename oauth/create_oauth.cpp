
#include <system.hpp>
#include <Classes.hpp>


#include "create_oauth.h"

#include "urlenc.h"
#include "hmac.h"


static AnsiString nonce;
static AnsiString timeStamp;
static AnsiString signature;

void makeNonce()
{
   static int times = 1000000;

   AnsiString time = Now().FormatString("yyyy/mm/dd-hh:nn:ss");
   char wk[1024];
   char wk2[1024];
   
   times++;
   AnsiString key = times;
   makeSign(time.c_str(), key.c_str(), wk);
   URLEnc(wk, wk2, true);
   nonce = wk2;
}

static int UnixTime(TDateTime aTime)
{
  const double aDay = 60.0 * 60.0 * 24.0;
  const int diffDayes = 25569;
  double wk = static_cast<double>(aTime);
  wk -= diffDayes;  // 基準となる日数を引いて
  wk *= aDay;       // 秒に換算して
  wk -= 32400;      // JST 基準なので、９時間分もどす
  return wk;
}

void makeTimeStamp()
{
   timeStamp = UnixTime(Now());
}

void makeSignHere(AnsiString baseText, AnsiString key)
{
  char wk[4096];
  char wk2[4096];
  makeSign(baseText.c_str(), key.c_str(), wk);
  URLEnc(wk, wk2);
  signature = wk2;
}


AnsiString makeBaseString(OAuthDataDef oauth, char *body, bool isImage)
{
  const char *url;
  char wk[10240];

  // アイコン変更が、base64エンコードのテキスト送信になったことに対する変更
  if (! body) // body がなければ、アイコン
    url = "https://api.twitter.com/1.1/account/update_profile_image.json";
  else if (isImage) // body つきのアイコンもある
    url = "https://api.twitter.com/1.1/account/update_profile_image.json";
  else  // post
	url = "https://api.twitter.com/1.1/statuses/update.json";

  AnsiString line = "POST&";

  URLEnc(url, wk);
  line += wk;
  line += "&";

  AnsiString baseLine = "";

  if(body && isImage)
  {
	baseLine += body;
	baseLine += "&";
  }

  baseLine += "oauth_consumer_key=";
  baseLine += oauth.Consumer_key;
  baseLine += "&";

  baseLine += "oauth_nonce=";
  baseLine += nonce;
  baseLine += "&";

  baseLine += "oauth_signature_method=HMAC-SHA1&";

  baseLine += "oauth_timestamp=";
  baseLine += timeStamp;
  baseLine += "&";

  baseLine += "oauth_token=";
  baseLine += oauth.Access_Token;
  baseLine += "&";

  if (body && (! isImage))
  {
    baseLine += "oauth_version=1.0&";
    baseLine += body;
  }
  else
  {
    baseLine += "oauth_version=1.0";
  }

  URLEnc(baseLine.c_str(), wk);
  line += wk;

  return line;
}


AnsiString makeHeadder(OAuthDataDef oauth)
{
  AnsiString line = "Authorization: OAuth ";

  line += "oauth_nonce=\"";
  line += nonce;
  line += "\", ";

  line += "oauth_signature_method=\"HMAC-SHA1\", ";

  line += "oauth_timestamp=\"";
  line += timeStamp;
  line += "\", ";

  line += "oauth_consumer_key=\"";
  line += oauth.Consumer_key;
  line += "\", ";

  line += "oauth_token=\"";
  line += oauth.Access_Token;
  line += "\", ";

  line += "oauth_signature=\"";
  line += signature;
  line += "\", ";

  line += "oauth_version=\"1.0\"";

  return line;
}

