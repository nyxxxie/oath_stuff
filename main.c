/*****************************************************************************\
Copyright (C) 2017 Nyxxie <nyxxxxie@gmail.com>
This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

/** Arg options */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  { NULL }
};

/** Arg datacontainer */
struct arguments {
    char* command;
    int verbose;
};

/** Argp option parser */
static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    struct arguments* arguments = state->input;

    switch (key) {
    case 'v':
        arguments->verbose = 1;
        break;

    case ARGP_KEY_ARG:
        /* Too many arguments. */
        if (state->arg_num >= 1) {
            argp_usage(state);
        }

        arguments->command = arg;
        break;

    case ARGP_KEY_END:
        /* Not enough arguments. */
        if (state->arg_num < 1) {
            argp_usage(state);
        }

        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Arp data */
const char* argp_program_version = "oath";
const char* argp_program_bug_address = "<nyxxxxie@gmail.com>";
static char doc[] = "implementation of some OATH algorithms HTOP and TOTP for OTP stuff.";
static char args_doc[] = "COMMAND";
static struct argp argp = {options, parse_opt, args_doc, doc};

/**
 * Entry point for program.
 */
int main(int argc, char** argv)
{
    struct arguments arguments;

    /* Parse args */
    arguments.verbose = 0;
    argp_parse (&argp, argc, argv, 0, 0, &arguments);

    printf("Command: \"%s\".\n", arguments.command);
    printf("\tverbose: %i\n", arguments.verbose);

    exit (0);
}
