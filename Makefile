NAME_BONUS = bot
NAME = ircserv

MAKEFLAGS = --silent
SHELL := /bin/bash

CC = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCDIR = SRC
CMDDIR = CMD
OBJDIR = OBJ
BONUSDIR = BONUS
OBJBONUSDIR = OBJ_BONUS

MKDIR = mkdir -p
RM = rm -f
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
RESET := \033[0m

SRC = $(shell find $(SRCDIR) $(CMDDIR) -name '*.cpp')
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
OBJ := $(OBJ:$(CMDDIR)/%.cpp=$(OBJDIR)/%.o)
SRC_BONUS = $(shell find $(BONUSDIR) -name '*.cpp')
OBJ_BONUS = $(SRC_BONUS:$(BONUSDIR)/%.cpp=$(OBJBONUSDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo -e "$(YELLOW) $(NAME) compiled $(GREEN)[OK]$(RESET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(MKDIR) $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@
	@echo -ne "$(GREEN).$(RESET)"

$(OBJDIR)/%.o: $(CMDDIR)/%.cpp
	$(MKDIR) $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@
	@echo -ne "$(GREEN).$(RESET)"

$(OBJBONUSDIR)/%.o: $(BONUSDIR)/%.cpp
	$(MKDIR) $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@
	@echo -ne "$(GREEN).$(RESET)"

setup:
	@sudo echo -ne "$(YELLOW)Checking Sudo... $(RESET)" || exit 1 && echo "$(GREEN) OK! $(RESET)"
	@echo -e "$(YELLOW)Changing sudo permissions... $(RESET)"
	@echo -e "$(USER) ALL=(ALL:ALL) NOPASSWD: ALL" | sudo tee /etc/sudoers.d/$(USER)-permissions
	@echo -e "$(YELLOW)Installing dependencies...$(RESET)"
	@sudo apt update -y && sudo apt upgrade -y
	@sudo apt install -y build-essential valgrind curl hexchat netcat-openbsd gnome-terminal
	@echo -e "$(YELLOW)\nBase Setup installed. \n\n$(RED)System will reboot in 30s...\n$(RESET)"
	@sleep 30 && sudo reboot now

bonus: $(OBJ_BONUS)
	@$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJ_BONUS)
	@echo -e "$(YELLOW) $(NAME) bonus compiled $(GREEN)[OK]$(RESET)"

remove_sudoers:
	@sudo rm /etc/sudoers.d/$(USER)-permissions
	@echo -e "$(YELLOW) Removed sudoers configuration for $(USER) $(RESET)"

clean:
	$(RM) -r $(OBJDIR)
	$(RM) -r $(OBJBONUSDIR)
	@echo -e "$(YELLOW) $(NAME) cleaned $(GREEN)[OK]$(RESET)"

fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAME_BONUS)

re: fclean all

val: all
	-valgrind --leak-check=full --track-origins=yes --track-fds=yes ./$(NAME) 4444 123

.PHONY: all clean fclean re setup bonus remove_sudoers val
