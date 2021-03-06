
#include "sekki.h"

// これはサンプル
// 実際には、この形で具体的なメッセージを適切なコードで記述してリンクすること。

sekki::nameDef const sekkiName[] = 
{
  {"春分", "《日天の中を行て昼夜等分の時也》"}, 
  {"清明", "《万物発して清浄明潔なれば、此芽は何の草としれる也》"},
  {"穀雨", "《春雨降りて百穀を生化すれば也》"},
  {"立夏", "《夏の立つがゆえ也》"},
  {"小満", "《万物盈満すれば草木枝葉繁る也》"},
  {"芒種", "《芒のある穀物、稼種する時也》"},
  {"夏至", "《陽熱至極し、また、日の長きのいたりなるを以て也》"},
  {"小暑", "《大暑来たれる前なれば也》"},
  {"大暑", "《暑気いたりつまりたるゆへなれば也》"},
  {"立秋", "《初めて秋の気立つがゆへなれば也》"},
  {"処暑", "《陽気とどまりて、初めて退きやまんとすれば也》"},
  {"白露", "《陰気ようやく重なりて露にごりて白色となれば也》"},
  {"秋分", "《陰陽の中分なれば也》"},
  {"寒露", "《陰寒の気に合って、露むすび凝らんとすれば也》"},
  {"霜降", "《つゆが陰気に結ばれて、霜となりて降るゆへ也》"},
  {"立冬", "《冬の気立ち初めて、いよいよ冷ゆれば也》"},
  {"小雪", "《冷ゆるが故に雨も雪となりてくだるがゆへ也》"},
  {"大雪", "《雪いよいよ降り重ねる折からなれば也》"},
  {"冬至", "《日南の限りを行て日の短きの至りなれば也》"},
  {"小寒", "《冬至より一陽起こるが故に陰気に逆らう、益々冷ゆる也》"},
  {"大寒", "《冷ゆることの至りて甚だしきときなれば也》"},
  {"立春", "《春の気立つを以て也》"},
  {"雨水", "《陽気地上に発し、氷雪とけて雨水となれば也》"},
  {"啓蟄", "《陽気地中にうごき、ちじまる虫、穴をひらき出れば也》"},
};

sekki::nameDef const zassetsuName[] = 
{
  {"土用入り", "《春、立ち前》"},  // index =  0 土用入り（春）
  {"土用入り", "《夏、立ち前》"},  // index =  1 土用入り（夏）
  {"土用入り", "《秋、立ち前》"},  // index =  2 土用入り（秋）
  {"土用入り", "《冬、立ち前》"},  // index =  3 土用入り（冬）

  {"入梅", "《梅雨、近きころ》"},  // index =  4 入梅
  {"半夏生", "《毒気降る故に、田畑に入るを禁ず日》"},  // index =  5 半夏生

  {"彼岸明け", "《彼岸（七日間）の終わる日》"}, // index =  6 彼岸開け
  {"彼岸明け", "《彼岸（七日間）の終わる日》"}, // index =  7 彼岸開け
  {"彼岸入り", "《彼岸（七日間）の始まる日》"}, // index =  8 彼岸入り
  {"彼岸入り", "《彼岸（七日間）の始まる日》"}, // index =  9 彼岸入り

  {"節分", "《春の節を分ける日・立春の前日》"}, //index = 10 節分
  {"八十八夜", "《立春より八十八日。遅霜の降るころ》"},   //index = 11 八十八夜

  {"二百十日", "《立春より二百十日。台風来るころ》"},     //index = 12 二百十日
  {"二百二十日", "《立春より二百二十日。台風来るころ》"}, //index = 13 二百二十日
  {"八朔", "《旧暦の八月一日・初穂などを贈り、感謝を表す日》"} // index = 14
  {"夏越", "《旧暦の六月晦日・半年の句切り》"}      // index = 15

};

sekki::nameDef const moonPhaseName[] = 
{ 
  {"朔（新月）", ""},
  {"上弦（半月）", ""},
  {"望（満月）", ""},
  {"下弦（半月）"""}
};

sekki::nameDef const kou72Name[] = 
{
  {"春分・初候・雀始巣","《すずめ はじめてすくう・雀が巣を構え始める》"},
  {"春分・次候・桜始開","《さくら はじめてひらく・桜の花が咲き始める》"},
  {"春分・末候・雷乃発声","《らい すなわちこえをはっす・遠くで雷の音がし始める》"},
  {"清明・初候・玄鳥至","《げんちょう いたる・燕が南からやって来る》"},
  {"清明・次候・鴻雁北","《こうがん きたす・雁が北へ渡って行く》"},
  {"清明・末候・虹始見","《にじ はじめてあらわる・雨の後に虹が出始める》"},
  {"穀雨・初候・葭始生","《よし はじめてしょうず・葦が芽を吹き始める》"},
  {"穀雨・次候・霜止出苗","《しもやんで なえいず・霜が終り稲の苗が生長する》"},
  {"穀雨・末候・牡丹華","《ぼたん はなさく・牡丹の花が咲く》"},
  {"立夏・初候・蛙始鳴","《かえる はじめてなく・蛙が鳴き始める》"},
  {"立夏・次候・蚯蚓出","《きゅういん いずる・蚯蚓（みみず）が地上に這出る》"},
  {"立夏・末候・竹笋生","《ちくかん しょうず・筍が生えて来る》"},
  {"小満・初候・蚕起食桑","《かいこ おこってくわをくらう・蚕が桑を盛んに食べ始める》"},
  {"小満・次候・紅花栄","《こうか さかう・紅花（べにばな）が盛んに咲く》"},
  {"小満・末候・麦秋至","《ばくしゅう いたる・麦が熟し麦秋となる》"},
  {"芒種・初候・螳螂生","《とうろう しょうず・螳螂（かまきり）が生まれ出る》"},
  {"芒種・次候・腐草為蛍","《ふそう ほたるとなる・腐った草が蒸れ蛍になる》"},
  {"芒種・末候・梅子黄","《うめのみ きなり・梅の実が黄ばんで熟す》"},
  {"夏至・初候・乃東枯","《ないとう かるる・夏枯草が枯れる》"},
  {"夏至・次候・菖蒲華","《しょうぶ はなさく・菖蒲（あやめ）の花が咲く》"},
  {"夏至・末候・半夏生","《はんげ しょうず・烏柄杓（からすびしゃく）が生える》"},
  {"小暑・初候・温風至","《おんぷう いたる・暖い風が吹いて来る》"},
  {"小暑・次候・蓮始開","《はす はじめてはなさく・蓮の花が開き始める》"},
  {"小暑・末候・鷹乃学習","《たか すなわちがくしゅうす・鷹の幼鳥が飛ぶことを覚える》"},
  {"大暑・初候・桐始結花","《きり はじめてはなをむすぶ・桐の実が生り始める》"},
  {"大暑・次候・土潤溽暑","《つちうるおいて じゅくしょす・土が湿って蒸暑くなる》"},
  {"大暑・末候・大雨時行","《たいう ときどきおこなう・時として大雨が降る》"},
  {"立秋・初候・涼風至","《りょうふう いたる・涼しい風が立ち始める》"},
  {"立秋・次候・寒蝉鳴","《かんせんなく・蜩（ひぐらし）が鳴き始める》"},
  {"立秋・末候・蒙霧升降","《もうむ しょうこう・深い霧が立ち込める》"},
  {"処暑・初候・綿柎開","《めんぷ ひらく・綿を包む咢(がく)が開く》"},
  {"処暑・次候・天地始粛","《てんち はじめてしゅくす・ようやく暑さが鎮まる》"},
  {"処暑・末候・禾乃登","《くわ すなわちのぼる・稲が実る》"},
  {"白露・初候・草露白","《そうろ しろし・草に降りた露が白く光る》"},
  {"白露・次候・鶺鴒鳴","《せきれい なく・鶺鴒（せきれい）が鳴き始める》"},
  {"白露・末候・玄鳥去","《げんちょう さる・燕が南へ帰って行く》"},
  {"秋分・初候・雷乃収声","《らい すなわちおさむ・雷が鳴り響かなくなる》"},
  {"秋分・次候・蟄虫坏戸","《ちっちゅう こをはいす・虫が土中に掘った穴をふさぐ》"},
  {"秋分・末候・水始涸","《みず はじめてかる・田畑の水を干し始める》"},
  {"寒露・初候・鴻雁来","《こうがん きたる・雁が飛来し始める》"},
  {"寒露・次候・菊花開","《きくはな ひらく・菊の花が咲く》"},
  {"寒露・末候・蟋蟀在戸","《しっそく こにあり・蟋蟀（こおろぎ）が戸の辺りで鳴く》"},
  {"霜降・初候・霜始降","《しも はじめてふる・霜が降り始める》"},
  {"霜降・次候・霎時施","《しぐれ ときどきほどこす・小雨がしとしと降る》"},
  {"霜降・末候・楓蔦黄","《ふうかつ きなり・もみじや蔦が黄葉する》"},
  {"立冬・初候・山茶始開","《さんちゃ はじめてひらく・山茶花が咲き始める》"},
  {"立冬・次候・地始凍","《ち はじめてこおる・大地が凍り始める》"},
  {"立冬・末候・金盞香","《きんせん こうばし・水仙の花が咲く》"},
  {"小雪・初候・虹蔵不見","《にじ かくれてみえず・虹を見かけなくなる》"},
  {"小雪・次候・朔風払葉","《さくふう はをはらふ・北風が木の葉を払い除ける》"},
  {"小雪・末候・橘始黄","《たちばな はじめてきなり・橘の葉が黄葉し始める》"},
  {"大雪・初候・閉塞成冬","《へいそくして ふゆとなる・天地の気が塞がって冬となる》"},
  {"大雪・次候・熊蟄穴","《くま あなにちっす・熊が冬眠のために穴に隠れる》"},
  {"大雪・末候・鮭魚群","《けつぎょ むらがる・鮭が群がり川を上る》"},
  {"冬至・初候・乃東生","《ないとう しょうず・夏枯草が芽を出す》"},
  {"冬至・次候・麋角解","《びかく げす・大鹿が角を落とす》"},
  {"冬至・末候・雪下出麦","《せつか むぎをいだす・雪の下で麦が芽を出す》"},
  {"小寒・初候・芹乃栄","《せり すなわちさかう・芹がよく生育する》"},
  {"小寒・次候・水泉動","《すいせん うごく・地中で凍った泉が動き始める》"},
  {"小寒・末候・雉始鳴","《きじ はじめてなく・雄の雉が鳴き始める》"},
  {"大寒・初候・款冬華","《ふきのはな さく・蕗の薹（ふきのとう）が蕾を出す》"},
  {"大寒・次候・水沢腹堅","《さわみず こおりつめる・沢に氷が厚く張りつめる》"},
  {"大寒・末候・鶏始乳","《にわとり はじめてとやにつく・鶏が卵を産み始める》"},
  {"立春・初候・東風解凍","《とうふう こおりをとく・東風が厚い氷を解かし始める》"},
  {"立春・次候・黄鶯見�v","《こうおう けんかんす・鶯が山里で鳴き始める》"},
  {"立春・末候・魚上氷","《うお こおりにのぼる・割れた氷の間から魚が飛び出る》"},
  {"雨水・初候・土脉潤起","《どみゃく うるおいおこる・雨が降って土が湿り気を含む》"},
  {"雨水・次候・霞始靆","《かすみ はじめてたなびく・霞がたなびき始める》"},
  {"雨水・末候・草木萠動","《そうもく きざしうごく・草木が芽吹き始める》"},
  {"啓蟄・初候・蟄虫啓戸","《ちっちゅう こをひらく・冬蘢りの虫が出て来る》"},
  {"啓蟄・次候・桃始笑","《もも はじめてわらう・桃の花が咲き始める》"},
  {"啓蟄・末候・菜虫化蝶","《なむし ちょうとけす・青虫が羽化して紋白蝶になる》"},
};

