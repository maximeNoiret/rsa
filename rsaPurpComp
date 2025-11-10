_rsa_completions()
{
    local cur prev commands help_subcommands
    COMPREPLY=()                                    # init completion reply
    cur="${COMP_WORDS[COMP_CWORD]}"                 # get current word
    prev="${COMP_WORDS[COMP_CWORD-1]}"              # get previous word
    commands="keygen encrypt decrypt help"          # possible general commands
    help_subcommands="help keygen encrypt decrypt"  # possible help sub commands

    # general commands suggestions
    if [[ $COMP_CWORD -eq 1 ]]; then
        COMPREPLY=( $(compgen -W "${commands}" -- "${cur}") )
        return 0
    fi

    # if first command is 'help', help sub commands suggestions
    if [[ $COMP_CWORD -eq 2 && "${COMP_WORDS[1]}" == "help" ]]; then
        COMPREPLY=( $(compgen -W "${help_subcommands}" -- "${cur}") )
        return 0
    fi

    return 0
}
complete -F _rsa_completions rsa
