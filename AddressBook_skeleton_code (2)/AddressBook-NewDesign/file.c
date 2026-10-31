#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
    FILE *fp = fopen("contacts.CSV", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp);
}
void loadContactsFromFile(AddressBook *addressBook) {
     FILE *fp = fopen("contacts.CSV", "r");
    if (!fp) {
        printf("No previous data to load.\n");
        return;
    }

    addressBook->contactCount = 0;
    while (fscanf(fp, " %[^,],%[^,],%[^\n]\n",
                  addressBook->contacts[addressBook->contactCount].name,
                  addressBook->contacts[addressBook->contactCount].phone,
                  addressBook->contacts[addressBook->contactCount].email) == 3) {
        addressBook->contactCount++;
    }

    fclose(fp);
    printf("Contacts loaded from file.\n");

}
