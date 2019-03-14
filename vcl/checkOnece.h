#if !defined(_BCL_CheckOnce)
#define      _BCL_CheckOnce

#include <U:/lib/pCount.h>

namespace onceToday
{

  const char *nameForCheck = "julo9kl9rogvpimvslb8t3e2"; // �����炭�ݒ肳��Ȃ��ł��낤�A�����_���ɐ��������萔

  const char *getName(const char *name) // �Öق̈����ɑ΂��鏈�����s��
  {
	return (name == 0) ? nameForCheck : name;
  }

  bool isDone(const char *name = 0)  // �󋵂��m�F���邾���ŁA�󋵂�ω������Ȃ��悤�ɕύX����
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

  void done(const char *name = 0) // �u�����͎��s�����v�Ə������ނ��߂̊֐�
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

  void reject(const char *name = 0) // �����I�Ɂu�����͂܂����s���Ă��Ȃ��v���Ƃɂ���B
  {
	pCount forCheck(getName(name));
	forCheck = 0;
  }

}

#endif
