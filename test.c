#include <stdio.h>
#include <math.h>

int	count_tokens(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return (0);
	while (tokens[i])
		i++;
	return (i);
}

int main()
{
	char *tokens[] = {"token1", "token2", "token3", NULL};
	printf("%d", count_tokens(tokens)); 
}