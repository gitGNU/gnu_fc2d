/*
GNU FC2D - A two time dimensional programing language.
Copyright (C) 2012  Free Software Foundation Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libfc2d/utils/arguments.h>
#include <libfc2d/utils/data-connect.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <stdlib.h>

typedef struct
{
  char* name;
  char* value;
  char cut;
  char* description;
  gboolean required;
  gboolean value_required;
  gboolean exists;
} fArgument;

typedef struct
{
  char* full_name;
  char* copyright;
  char* description;
  char* syntax;
  char* program_name;
  int argc;
  char** argv;
  gboolean official_gnu;
  gchar* report_bugs;
  gchar* pkg_homepage;
  gboolean additional_information;
} fArgsInfo;

gint
f_arg_process_name_compare (fArgument* arg, char* name)
{
    return g_strcmp0 (arg->name, name);
}

void
f_arg_process (int argc, char** argv,
               const char* full_name,
               const char* copyright,
               const char* description,
               const char* syntax
              )
{
  fArgsInfo* info;
  int i, j;
  GList* l;
  GList* l2 = NULL;
  fArgument* arg;

  info = f_data_get(0, "args_info");
  
  if( info == NULL ) 
  {
    info = g_malloc0 (sizeof (fArgsInfo));
    f_data_connect (0, "args_info", info);
  }
  
  f_arg_add ("version", 'v',
             "Display version information", FALSE,
             FALSE);

  f_arg_add ("help", 'h',
             "Display this information", FALSE,
             FALSE);

  info->full_name = full_name;
  info->copyright = copyright;
  info->description = description;
  info->syntax = syntax;
  info->argc = argc;
  info->argv = argv;

  if (argc > 0)
    info->program_name = argv[0];

  for (i = 1; i < argc; i++)
    {

      if (strlen (argv[i]) > 2 && argv[i][0] == '-' &&
          argv[i][1] == '-')
        {
          l = f_data_get (0, "args");

          for (; l != NULL; l = l->next)
            {
              arg = l->data;
              if (g_strcmp0 (&(argv[i][2]), arg->name) == 0)
                {
                  arg->exists = TRUE;
                  if (arg->value_required == TRUE)
                    {
                      if (i + 1 < argc && argv[i + 1][0] != '-')
                        {
                          arg->value = argv[i + 1];
                          i++;
                        }
                      else
                        {
                          f_arg_help ();
                          exit (1);
                        }

                    }
                  break;
                }
            }
        }
      else if (strlen (argv[i]) >= 2 && argv[i][0] == '-')
        {
            l = f_data_get (0, "args");

            for( j = 1; j < strlen(argv[i]) && i < argc; j++ )
            {
                for (; l != NULL; l = l->next)
                    {
                    arg = l->data;
                    if (argv[i][j] == arg->cut)
                        {
                        arg->exists = TRUE;
                        if (arg->value_required == TRUE)
                            {
                            if (i + 1 < argc && argv[i + 1][0] != '-')
                                {
                                    arg->value = argv[i + 1];
                                    i++;
                                }
                            else
                                {
                                    f_arg_help ();
                                    exit (1);
                                }

                            } else
                            {
                                if( i + 1 < argc && argv[i + 1][0] != '-')
                                {
                                    l2 = f_data_get(0, "user_args");
                                    l2 = g_list_append( l2, argv[i + 1] );
                                    f_data_connect( 0, "user_args", l2 );
                                }
                            }
                        break;
                        }
                    }
            }
          
            if( i >= argc ) 
            {
              printf(_("Options that require parameters, appear to be\n"
                       "being used in ways not yet supported\n"));
            }
        } else {
            l2 = f_data_get(0, "user_args");
            l2 = g_list_append( l2, argv[i] );
            f_data_connect( 0, "user_args", l2 );
        }

    }

  if (f_arg_exists ("help"))
    {
      f_arg_help ();
      exit (0);
    }

  if (f_arg_exists ("version"))
    {
      printf ("%s\n\n", info->full_name);
      printf ("Copyright (C) %s\n", info->copyright);
      printf ("License GPLv3+: GNU GPL version 3 or later"
              " <http://gnu.org/licenses/gpl.html>\n"
              "This is free software: you are free to change and"
              " redistribute it.\nThere is NO WARRANTY, to the "
              "extent permitted by law.\n\n");
      exit (0);
    }

}

char*
f_option_get (const char* arg)
{
  GList* l;
  fArgument* args;

  l = f_data_get (0, "args");
  for (; l != NULL; l = l->next)
    {
      args = l->data;

      if (g_strcmp0 (arg, args->name) == 0)
        return args->value;
    }

  return NULL;
}

char*
f_arg_get (guint id)
{
  GList* user_args;
  guint i = 0;

  user_args = f_data_get (0, "user_args");
  for ( ; user_args != NULL && i < id; user_args = user_args->next ) i++;
  
  return user_args->data;
  
}

guint
f_args_len ()
{
  GList* user_args;
  guint i = 0;

  user_args = f_data_get (0, "user_args");
  for ( ; user_args != NULL; user_args = user_args->next ) i++;

  return i;
}

void
f_arg_add (const char* arg, char cut,
           const char* d, gboolean required,
           gboolean with_value)
{
  fArgument* a;
  GList* l;

  a = g_malloc0 (sizeof (fArgument));

  l = f_data_get (0, "args");
  l = g_list_append (l, a);
  f_data_connect (0, "args", l);

  a->name = arg;
  a->cut = cut;
  a->description = d;
  a->required = required;
  a->value_required = with_value;
}

gboolean
f_arg_exists (const char* arg)
{
  GList* l;
  fArgument* argument;

  l = f_data_get (0, "args");

  if (l == NULL)
    return FALSE;
    
  l = g_list_find_custom ( l, arg,
                           f_arg_process_name_compare);
  argument = l->data;
  
  if( argument && argument->exists )
    return TRUE;

  return FALSE;
}

void
f_arg_help ()
{
  fArgsInfo* info;
  fArgument* args;
  GList* l;
  char* str[1000];
  int i, j;
  int line_size=9;

  info = f_data_get (0, "args_info");
  l = f_data_get (0, "args");

  printf ("%s\n\n", info->full_name);
  printf ("Copyright (C) %s\n", info->copyright);
  printf ("License GPLv3+: GNU GPL version 3 or later"
          " <http://gnu.org/licenses/gpl.html>\n"
          "This is free software: you are free to change and"
          " redistribute it.\nThere is NO WARRANTY, to the "
          "extent permitted by law.\n\n");

  printf ("Syntax: %s ", info->program_name);

  l = f_data_get (0, "args");
  for (; l != NULL; l = l->next)
    {
      args = l->data;

      if (args->required)
        {
          if (args->value_required)
          {
            printf ("-%c <value> ", args->cut);
            line_size += 11;
          }
          else 
          {
            printf ("-%c ", args->cut);
            line_size += 3;
          }
        } else 
        {
            if (args->value_required)
            {
                printf ("[-%c <value>] ", args->cut);
                line_size += 13;
            }
            else 
            {
                printf ("[-%c] ", args->cut);
                line_size += 5;
            }
        }
        
        if( line_size > 60 )
            printf("\n");
    }

    if( info->syntax != NULL )
        printf ("%s", info->syntax);
    
    printf("\n");
  
    if( info->description != NULL )
        printf("%s", info->description);
    
    printf("\n\n");

  printf ("Options:\n\n");

  i = 0;
  l = f_data_get (0, "args");
  for (; l != NULL; l = l->next)
    {
      args = l->data;

      if (strlen (args->name) > i)
        i = strlen (args->name);
    }

  i += 10;
  l = f_data_get (0, "args");
  for (; l != NULL; l = l->next)
    {
      args = l->data;

      sprintf (str, " -%c, --%s", args->cut, args->name);
      printf ("%s", str);

      for (j = 0; j < i - strlen (str); j++)
        {
          printf (" ");
        }

      printf ("%s\n", args->description);
    }
    
    if( info->additional_information ) 
    {
        printf( "\nReport bugs to: %s\n"
                "pkg home page: <%s>\n", info->report_bugs, info->pkg_homepage );
        
        if( info->official_gnu ) 
        {
            printf("General help using GNU software:"
            " <http://www.gnu.org/gethelp/>\n");
        }
    }

}

void 
additional_information( gboolean official_gnu, 
                        gchar* report_bugs,
                        gchar* pkg_homepage )
{
    fArgsInfo* info;

    info = f_data_get(0, "args_info");
    
    if( info == NULL ) 
    {
        info = g_malloc0 (sizeof (fArgsInfo));
        f_data_connect (0, "args_info", info);
    }
    
    info->official_gnu = official_gnu;
    info->report_bugs = report_bugs;
    info->pkg_homepage = pkg_homepage;
    info->additional_information = TRUE;
}
