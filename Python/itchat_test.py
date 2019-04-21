import itchat
import time

itchat.auto_login(True)

SINCERE_WISH=u'祝%s新年快乐'

friendList=itchat.get_friends(update=True)[1:]

print(friendList)
# count = 0
# for friend in friendList:
#     itchat.send( SINCERE_WISH % (friend['DisplayName']
#          or friend['NickName']), friend['UserName'])
#     time.sleep(5)
# print("----end----")