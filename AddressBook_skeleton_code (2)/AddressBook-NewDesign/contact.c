#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include <ctype.h>

int valid_name(const char *name) 
{
    for (int i = 0; name[i]; i++) 
    {
        if (!isalpha(name[i]) && name[i] != ' ')
            return 0;
    }
    return 1;
}

int valid_phone(const char *phone) 
{
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; i++)
        if (!isdigit(phone[i])) return 0;
    return 1;
}

int is_unique_phone(AddressBook *addressBook, const char *phone) 
{
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
            return 0;
    }
    return 1;
}

int valid_email(const char *email) 
{
    const char *at = strchr(email, '@');
    if (!at || at == email) 
    return 0;
    const char *dot = strchr(at, '.');
    if (!dot || dot == at + 1 || dot[1] == '\0') 
    return 0;
    return 1;
}
int isEmailUnique(const AddressBook *addressBook, const char *email) 
{
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0)
            return 0; // Duplicate found
    }
    return 1; // Unique
}

void createContact(AddressBook *addressBook) 
{
    if (addressBook->contactCount >= MAX_CONTACTS) 
    {
        printf("Address Book full.\n");
        return;
    }

    Contact c;

    do 
    {
        printf("Enter name: ");
        scanf(" %[^\n]", c.name);
        if (!valid_name(c.name)) 
        printf("Invalid name. Try again.\n");
    } 
    while (!valid_name(c.name));

    do 
    {
        printf("Enter phone (10 digits, unique): ");
        scanf(" %[^\n]", c.phone);
        if (!valid_phone(c.phone))
         printf("Invalid phone. Try again.\n");
        else if (!is_unique_phone(addressBook, c.phone))
         printf("Phone must be unique. Try again.\n");
    }
     while (!valid_phone(c.phone) || !is_unique_phone(addressBook, c.phone));

    do 
    {
        printf("Enter email: ");
        scanf(" %[^\n]", c.email);
        if (!valid_email(c.email))
        { 
        printf("Invalid email. Try again.\n");
        }
        else if(!isEmailUnique(addressBook,c.email))
        {
        printf("Email already exists,Enter unique one.\n");
        }
        else
        {
        break;
        }
    } 

    while (1);
    addressBook->contacts[addressBook->contactCount++] = c;
    printf("Contact added!\n");
}

void listContacts(AddressBook *addressBook, int field) 
{
    if (addressBook->contactCount == 0) {
        printf("No contacts available.\n");
        return;
    }
    int sortChoice;

    printf("\n-----------------------------------------Contact List------------------------------------------\n");
    printf("\n|%-5s | %-25s | %-25s | %-30s|\n","S.No","Name","phone","email");
    printf("-----------------------------------------------------------------------------------------------\n");


    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        printf("|%-5d | %-25s | %-25s | %-30s|\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
        printf("-----------------------------------------------------------------------------------------------\n");
    }
    for (int i = 0; i < addressBook->contactCount - 1; i++) 
    {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++) 
        {
            int cmp = 0;
            switch (field) 
            {
                case 1:
                    cmp = strcmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name);
                    break;
                case 2:
                    cmp = strcmp(addressBook->contacts[j].phone, addressBook->contacts[j + 1].phone);
                    break;
                case 3:
                    cmp = strcmp(addressBook->contacts[j].email, addressBook->contacts[j + 1].email);
                    break;
            }
            if (cmp > 0) {
                Contact temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    populateAddressBook(addressBook);
}
    

void saveAndexit(AddressBook *addressBook) {
    saveContactsToFile(addressBook);
    exit(EXIT_SUCCESS);
}

void searchContact(AddressBook *addressBook) 
{
   
    if (addressBook->contactCount == 0) 
    {
        printf("No contacts to search.\n");
        return;
    }

    int option;
    char query[100];
    printf("Search by: 1. Name  2. Phone  3. Email\n");
    printf("Enter option: ");
    scanf("%d", &option);

    printf("Enter search value: ");
    scanf(" %[^\n]", query);

    int found = 0;
    for (int i = 0; i < addressBook->contactCount; i++) 
    {
        int match = 0;

        switch (option) 
        {
            case 1:
                if (strcasecmp(addressBook->contacts[i].name, query)) 
                match = 1;
                break;
            case 2:
                if (strcmp(addressBook->contacts[i].phone, query) == 0) 
                match = 1;
                break;
            case 3:
                if (strcasecmp(addressBook->contacts[i].email, query)) 
                match = 1;
                break;
            default:
                printf("Invalid search option.\n");
                return;
        }

        if (match) 
        {
            found = 1;
            printf("\n%d:\n", i + 1);
            printf("\n-----------------------------------------Contact List-------------------------------------------\n");
    printf("\n|%-5s | %-25s | %-25s | %-30s|\n","S.No","Name","phone","email");
    printf("------------------------------------------------------------------------------------------------\n");
    printf("|%-5d | %-25s | %-25s | %-30s|\n",i+1,
        addressBook->contacts[i].name,addressBook->contacts[i].phone,addressBook->contacts[i].email);
    printf("------------------------------------------------------------------------------------------------\n");
        }
    }

    if (!found) {
        printf("No matching contact found.\n");
    }
}


void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
      if (addressBook->contactCount == 0) {
        printf("No contacts to edit.\n");
        return;
    }

    char name[20];
    printf("Enter the name of the contact to edit: ");
    scanf(" %[^\n]", name);

    int found = -1;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].name, name) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Contact with name %s not found.\n", name);
        return;
    }

    Contact *c = &addressBook->contacts[found];
    printf("Editing contact: %s, %s, %s\n", c->name, c->phone, c->email);

    //Ask name from the user
    do {
        printf("Enter new name (or press Enter to keep '%s'): ", c->name);
        char newName[100];
        fgets(newName, sizeof(newName), stdin); // flush newline
        fgets(newName, sizeof(newName), stdin);
        newName[strcspn(newName, "\n")] = '\0'; // Remove newline
        if (strlen(newName) == 0) break;
        if (valid_name(newName)) {
            strcpy(c->name, newName);
            break;
        } else {
            printf("Invalid name. Try again.\n");
        }
    } while (1);

    // Update Phone
    do {
        printf("Enter new phone (or press Enter to keep '%s'): ", c->phone);
        char newPhone[20];
        fgets(newPhone, sizeof(newPhone), stdin);
        newPhone[strcspn(newPhone, "\n")] = '\0';
        if (strlen(newPhone) == 0) break;
        if (valid_phone(newPhone) && is_unique_phone(addressBook, newPhone)) {
            strcpy(c->phone, newPhone);
            break;
        } else {
            printf("Invalid or duplicate phone number. Try again.\n");
        }
    } while (1);

    // Update Email
    do {
        printf("Enter new email (or press Enter to keep '%s'): ", c->email);
        char newEmail[100];
        fgets(newEmail, sizeof(newEmail), stdin);
        newEmail[strcspn(newEmail, "\n")] = '\0';
        if (strlen(newEmail) == 0) break;
        if (valid_email(newEmail) && isEmailUnique(addressBook, newEmail)) {
            strcpy(c->email, newEmail);
            break;
        } else {
            printf("Invalid or duplicate email. Try again.\n");
        }
    } while (1);

    printf("Contact updated successfully!\n");
}
    


void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
     if (addressBook->contactCount == 0) {
        printf("No contacts to delete.\n");
        return;
    }

    int index,field;
    listContacts(addressBook,field);
    printf("Enter index of contact to delete (1 - %d): ", addressBook->contactCount);
    scanf("%d", &index);
    if (index < 1 || index > addressBook->contactCount) 
    {
        printf("Invalid index.\n");
        return;
    }

    for (int i = index - 1; i < addressBook->contactCount - 1; i++) 
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;
    printf("Contact deleted.\n");
}