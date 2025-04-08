static int	ft_end_of_word(char *s, char c)
{
	int	i;

	ft_printerr("PING ft_end_of_word 0: %s\n", s);
	if (s[0] == c && c != ' ')
		return (2);
	i = -1;
	while (s[++i])
	{
		while (ft_isspace(s[i]))
			i++;
		ft_printerr("PING ft_end_of_word 1\n");
		if (c == '\'' || c == '"')
		{
			if (s[i] == c && s[i - 1] == '\\')
				i++;
			else if (s[i] == c)
				break ;
		}
		else
			if (ft_ismeta("|><&$\"'()*=", s[i]))
				return (i + 1);
		ft_printerr("PING ft_end_of_word 2\n");
		if (s[i] == ' ' && c == ' ')
			return (i + 1);
		ft_printerr("PING ft_end_of_word 3\n");
		if (s[i] == '$' && c == '"')
			return (i + 1);
		ft_printerr("PING ft_end_of_word 4\n");
		
	}
	if (!s[i] && c != ' ')
		return (i + 1);
	ft_printerr("PING ft_end_of_word 5\n");
	return (i + 2);
}