#if !defined(_BCL_CheckOnce)
#define      _BCL_CheckOnce

#include <U:/lib/pCount.h>

namespace onceToday
{

  const char *nameForCheck = "julo9kl9rogvpimvslb8t3e2"; // おそらく設定されないであろう、ランダムに生成した定数

  const char *getName(const char *name) // 暗黙の引数に対する処理を行う
  {
	return (name == 0) ? nameForCheck : name;
  }

  bool isDone(const char *name = 0)  // 状況を確認するだけで、状況を変化させないように変更した
  {
	pCount forCheck(getName(name));

	if (static_cast<int>(Date()) == forCheck.get())
	  return true;
	else
	   return false;
  }

  bool isFirst(const char *name = 0)
  {
	return (! isDone(name));
  }

  void done(const char *name = 0) // 「今日は実行した」と書き込むための関数
  {
	pCount forCheck(getName(name));
	forCheck = static_cast<int>(Date());
  }

  bool isFirst_update(const char *name = 0)
  {
	bool wk = isFirst(name);
	done(name);
	return wk;
  }

  void reject(const char *name = 0) // 強制的に「今日はまだ実行していない」ことにする。
  {
	pCount forCheck(getName(name));
	forCheck = 0;
  }

}

#endif
