# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   wildcard.mk                                        :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: jjorda <jjorda@student.42.fr>              +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2025/08/07 23:20:00 by jjorda            #+#    #+#             #
#   Updated: 2025/08/07 23:20:00 by jjorda           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

# ================================= PHASE 9: WILDCARDS ================================= #

# Test basique des wildcards
test_wildcard: $(NAME)
	@echo "$(GREEN)🧪 Tests Wildcards de base$(NC)"
	@./test_wildcard.sh

# Tests avancés des wildcards 
test_wildcard_advanced: $(NAME)
	@echo "$(GREEN)🔬 Tests Wildcards avancés$(NC)"
	@./test_wildcard_advanced.sh

# Test spécifique phase 9
test_phase_9: $(NAME)
	@echo "$(GREEN)🧪 Phase 9: Tests Wildcards$(NC)"
	@echo "$(YELLOW)Testing wildcard expansion (*)$(NC)"
	@./test_wildcard.sh

# Test d'intégration avec le reste du système
test_wildcard_integration: $(NAME)
	@echo "$(GREEN)🔗 Test intégration wildcards$(NC)"
	@mkdir -p test_integration
	@cd test_integration && touch test1.c test2.c main.h parser.h
	@cd test_integration && echo "echo *.c | wc -w" | ../$(NAME) > minishell_out.txt
	@cd test_integration && echo "echo *.c | wc -w" | bash > bash_out.txt
	@cd test_integration && diff minishell_out.txt bash_out.txt > /dev/null && echo "$(GREEN)✅ Wildcards + Pipes OK$(NC)" || echo "$(RED)❌ Problème intégration$(NC)"
	@rm -rf test_integration

# Tests avec d'autres fonctionnalités bonus
test_wildcard_heredoc: $(NAME)
	@echo "$(GREEN)🔗 Test Wildcards + Heredoc$(NC)"
	@mkdir -p test_wh && cd test_wh && touch file1.txt file2.txt
	@cd test_wh && printf "cat << EOF\n*.txt\nEOF\n" | ../$(NAME) > minishell_out.txt
	@cd test_wh && printf "cat << EOF\n*.txt\nEOF\n" | bash > bash_out.txt
	@cd test_wh && diff minishell_out.txt bash_out.txt > /dev/null && echo "$(GREEN)✅ Wildcard + Heredoc OK$(NC)" || echo "$(YELLOW)⚠️  Différence détectée$(NC)"
	@rm -rf test_wh

# Tests de performance wildcards
test_wildcard_performance: $(NAME)
	@echo "$(GREEN)⚡ Test performance wildcards$(NC)"
	@mkdir -p test_perf
	@cd test_perf && for i in {1..100}; do touch file$$i.txt; done
	@cd test_perf && time (echo "echo *.txt | wc -w" | ../$(NAME) > /dev/null)
	@rm -rf test_perf

# Nettoyage spécifique wildcards
clean_wildcard:
	@rm -f test_wildcard_*.txt test_integration
	@rm -rf test_*
	@echo "$(GREEN)🧹 Nettoyage wildcard terminé$(NC)"

# Tests complets wildcards
test_wildcard_all: test_wildcard test_wildcard_integration test_wildcard_heredoc
	@echo "$(GREEN)🎉 Tous les tests wildcards terminés$(NC)"

.PHONY: test_wildcard test_wildcard_advanced test_phase_9 test_wildcard_integration \
        test_wildcard_heredoc test_wildcard_performance clean_wildcard test_wildcard_all
