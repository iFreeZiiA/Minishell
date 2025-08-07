#!/bin/bash
# test_heredoc.sh - Test du module heredoc

echo "============================================="
echo "            TEST HEREDOC MINISHELL"
echo "============================================="

TOTAL_TESTS=0
PASSED_TESTS=0

test_heredoc() {
    local test_name="$1"
    local command="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    echo ""
    echo "Test $TOTAL_TESTS: $test_name"
    echo "Command: $command"
    
    # Exécuter avec bash et minishell
    bash_result=$(echo -e "$command" | bash 2>/dev/null | wc -l)
    mini_result=$(echo -e "$command" | ./minishell 2>/dev/null | wc -l)
    
    if [ "$bash_result" = "$mini_result" ]; then
        echo "✅ PASSED (${bash_result} lines output)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo "❌ FAILED"
        echo "   Bash lines: $bash_result"
        echo "   Mini lines: $mini_result"
    fi
}

echo ""
echo "🧪 Tests de base heredoc..."

test_heredoc "Heredoc simple" "cat << EOF\nhello\nworld\nEOF"

test_heredoc "Heredoc avec pipe" "cat << EOF | wc -l\nline1\nline2\nline3\nEOF"

test_heredoc "Heredoc délimiteur custom" "cat << DELIM\ntest line\nanother line\nDELIM"

test_heredoc "Heredoc vide" "cat << EOF\nEOF"

test_heredoc "Heredoc avec variables" "cat << EOF\nhello \$USER\nEOF"

echo ""
echo "============================================="
echo "              RÉSUMÉ HEREDOC"
echo "============================================="
echo "Tests passés: $PASSED_TESTS/$TOTAL_TESTS"
echo "Taux de réussite: $(( PASSED_TESTS * 100 / TOTAL_TESTS ))%"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo "🎉 HEREDOC TOTALEMENT FONCTIONNEL !"
else
    echo "⚠️  Heredoc partiellement fonctionnel"
fi

echo ""
