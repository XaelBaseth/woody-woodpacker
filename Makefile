#--------------------------------------------Variables--------------------------------------------

MAKEFLAGS	+=	--silent

NAME		=	woody_woodpacker
INC			=	inc/
HEADER		=	-I inc
SRC_DIR 	=	src/
OBJ_DIR 	=	obj/
CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror -g -fsanitize=address
NASM		=	nasm
NASMFLAGS	=	-f elf64
LIBFT		=	lib/libft/
RM			=	rm -rf
ECHO		=	echo

#--------------------------------------------Colors--------------------------------------------

DEF_COLOR	=	\033[0;39m
ORANGE		=	\033[0;33m
GRAY		=	\033[0;90m
RED			=	\033[0;91m
GREEN		=	\033[1;92m
YELLOW		=	\033[1;93m
BLUE		=	\033[0;94m
MAGENTA		=	\033[0;95m
CYAN		=	\033[0;96m
WHITE		=	\033[0;97m
CLEARLINE	=	\033[1A\033[K

#--------------------------------------------Files--------------------------------------------

MAIN_DIR	=	main/
MAIN_FILES	=	woody_woodpacker utils

ELF_DIR		=	elf/
ELF_FILES	=	checkers endian

ENCR_DIR	=	encryption/
ENCR_FILES	=	encryption injection segment

ASM_DIR		=	asm/
ASM_FILES	=	encrypt

SRC_MAI_FILE=	$(addprefix $(MAIN_DIR), $(MAIN_FILES))
SRC_ELF_FILE=	$(addprefix $(ELF_DIR), $(ELF_FILES))
SRC_ENC_FILE=	$(addprefix $(ENCR_DIR), $(ENCR_FILES))
SRC_ASM_FILE=	$(addprefix $(ASM_DIR), $(ASM_FILES))

MSRC		=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_MAI_FILE)))
MOBJ		=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_MAI_FILE)))

ELFSRC		=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_ELF_FILE)))
ELFOBJ		=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_ELF_FILE)))

ENCSRC		=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_ENC_FILE)))
ENCOBJ		=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_ENC_FILE)))

ASMSRC		=	$(addprefix $(SRC_DIR), $(addsuffix .s, $(SRC_ASM_FILE)))
ASMOBJ		=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_ASM_FILE)))

OBJF		=	.cache_exists

OBJ 		=	$(MOBJ) $(ELFOBJ) $(ENCOBJ) $(ASMOBJ)

#--------------------------------------------Rules--------------------------------------------

all:           	message $(NAME)

message: ## Display the building of files.
					@echo "\n$(YELLOW)[Starting to build...]$(DEF_COLOR)\n\n$(MAGENTA)"

$(NAME):		$(OBJ) $(OBJF)
					@make -C $(LIBFT)
					@cp $(LIBFT)/libft.a .
					@$(RM) $(LIBFT)/libft.a
					@$(CC) $(CFLAGS) $(OBJ) $(HEADER) libft.a -o $(NAME)
					@$(ECHO) "$(YELLOW)[WOODY_WOODPACKER]:\t$(ORANGE)[==========]\t$(GREEN) => Success!$(DEF_COLOR)\n"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(OBJF)
					@$(CC) $(CFLAGS) -c $< -o $@
					@$(ECHO) "$(CLEARLINE)$< created"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.s $(OBJF)
					@$(NASM) $(NASMFLAGS) -s $< -o $@
					@$(ECHO) "$(CLEARLINE)$< assembled"

$(OBJF):
					@mkdir -p $(OBJ_DIR)
					@mkdir -p $(OBJ_DIR)$(MAIN_DIR)
					@mkdir -p $(OBJ_DIR)$(ELF_DIR)
					@mkdir -p $(OBJ_DIR)$(ENCR_DIR)
					@mkdir -p $(OBJ_DIR)$(ASM_DIR)
					@touch $(OBJF)

help: ## Print help on Makefile.
					@grep '^[^.#]\+:\s\+.*#' Makefile | \
					sed "s/\(.\+\):\s*\(.*\) #\s*\(.*\)/`printf "$(GRAY)"`\1`printf "$(DEF_COLOR)"`	\3 /" | \
					expand -t8

clean: ## Clean generated files and cache.
					@$(RM) $(OBJ_DIR)
					@$(RM) $(OBJF)
					@$(RM) $(LIBFT)/obj
					@$(ECHO) "$(BLUE)[WOODY_WOODPACKER]:\tobject files$(DEF_COLOR)\t$(GREEN) => Cleaned!$(DEF_COLOR)\n"

fclean: ## Clean all generated file, including binaries.
					@make clean
					@$(RM) $(NAME) libft.a woody
					@make fclean -C $(LIBFT)
					@$(ECHO) "$(CYAN)[WOODY_WOODPACKER]:\texec. files$(DEF_COLOR)\t$(GREEN) => Cleaned!$(DEF_COLOR)\n"

re: ## Clean and rebuild binary file.
					@make fclean all
					@$(ECHO) "\n$(GREEN)###\tCleaned and rebuilt everything for [WOODY_WOODPACKER]!\t###$(DEF_COLOR)\n"

.PHONY:			all clean fclean re message help