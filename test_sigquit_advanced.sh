#!/bin/bash

echo "Testing SIGQUIT behavior in minishell vs bash"
echo "============================================="

echo ""
echo "Test avec bash (envoie SIGQUIT à sleep) :"
bash -c 'sleep 10 &
SLEEP_PID=$!
sleep 0.5
kill -QUIT $SLEEP_PID
wait $SLEEP_PID
echo "Exit code: $?"'

echo ""
echo "Test avec minishell (envoie SIGQUIT à sleep) :"
printf "sleep 10\nexit\n" | ./minishell &
MINISHELL_PID=$!
sleep 0.5
# Trouver le processus sleep lancé par minishell
SLEEP_PID=$(pgrep -P $MINISHELL_PID sleep)
if [ -n "$SLEEP_PID" ]; then
    echo "Envoi de SIGQUIT à sleep (PID: $SLEEP_PID)"
    kill -QUIT $SLEEP_PID
fi
wait $MINISHELL_PID
echo "Exit code du minishell: $?"
