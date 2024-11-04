#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
 
char* encode(char *str) {
  int length = strlen(str);
	char *encoded = (char*)malloc(length);
	int k = 0;

  for (int i = 0; str[i]; ++i) {
    if (str[i] > 64 && str[i] < 91) {
      encoded[k++] = str[i] - 23;
    } else {
    encoded[k++] = str[i] + 4;
    }
  }

  return encoded;
}

// diable echo
void offEcho() {
  struct termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  tty.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

// enable echo
void onEcho() {
  struct termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  tty.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);

}

void getstr(int fd, int maxInfoSize, char *catable, char *info) {
  fgets(info, maxInfoSize, stdin);
  info[strlen(info) - 1] = '\0';

  if (!(strcmp(catable, "Password: "))) {
    info = encode(info);
  }

  write(fd, strcat(catable, info), strlen(catable));
  write(fd, "\n", 1);

  return;
}

int main() {
  int fd = open("registration.txt", O_RDWR | O_TRUNC | O_APPEND);
  const int maxInfoSize = 70;
  char info[maxInfoSize];
  char name[maxInfoSize] = "Name: ";
  char surname[maxInfoSize] = "Surname: ";
  char email[maxInfoSize] = "Email: ";
  char phone[maxInfoSize] = "Phone Number: ";
  char password[maxInfoSize] = "Password: ";

  // name
  puts("Input name:");
  getstr(fd, maxInfoSize, name, info);

  // surname
  puts("Input surname:");
  getstr(fd, maxInfoSize, surname, info);

  // email
  puts("Input email:");
  getstr(fd, maxInfoSize, email, info);

  // phone number
  puts("Input phone number (0xx) xxx xxx:");
  getstr(fd, maxInfoSize, phone, info);

  // password
  system("clear");
  puts("Input Password");
  offEcho();
  getstr(fd, maxInfoSize, password, info);
  onEcho();

  close(fd);
  return 0;
}