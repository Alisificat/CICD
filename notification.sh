#!/bin/bash
#Все переменные и константы начинающиеся с CI_ это глобальные переменные gitlab-runner
STAGE_TYPE=$1
BOT_TOKEN="7115903726:AAGdBSAyZG6mvFDfj8ychvqyM6VxkB6I-Pw"
CHAT_ID="853370838"

sleep 5

if [ "$CI_JOB_STATUS" == "success" ]; then
  MESSAGE="✅ Стадия $STAGE_TYPE 👉$CI_JOB_NAME👈 успешно завершена ✅ $CI_PROJECT_URL/pipelines"
else
  MESSAGE="🚫 Стадия $STAGE_TYPE 👉$CI_JOB_NAME👈 завершилась с ошибкой! 🚫 $CI_PROJECT_URL/pipelines"
fi

curl -X POST "https://api.telegram.org/bot$BOT_TOKEN/sendMessage" -d chat_id=$CHAT_ID -d text="$MESSAGE"