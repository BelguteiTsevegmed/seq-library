#include "seq.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK_NULL1(x)                                                         \
  do {                                                                         \
    if (x == NULL) {                                                           \
      errno = ENOMEM;                                                          \
      return -1;                                                               \
    }                                                                          \
  } while (0)

#define CHECK_NULL2(x)                                                         \
  do {                                                                         \
    if (x == NULL) {                                                           \
      errno = ENOMEM;                                                          \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

/* The struct that represents the set of sequences */
typedef struct seq {
  char *class_name;
  char *seq;
  struct seq *right;
  struct seq *mid;
  struct seq *left;
} seq_t;

/* Creates a new seq_t set */
seq_t *seq_new(void) {
  seq_t *seq = malloc(sizeof(seq_t));
  CHECK_NULL2(seq);
  seq->seq = NULL;
  seq->class_name = NULL;
  seq->left = NULL;
  seq->mid = NULL;
  seq->right = NULL;
  return seq;
}

/* Frees all memory associated with the seq_t set */
void seq_delete(seq_t *p) {
  if (p == NULL)
    return;
  seq_delete(p->left);
  seq_delete(p->mid);
  seq_delete(p->right);
  free(p->seq);
  if (p->class_name)
    free(p->class_name);
  free(p);
}

/* Checks if the parameters are valid */
bool check_params(seq_t *p, char const *s) {
  /* Check if the parameters are empty */
  if (p == NULL || s == NULL || s[0] == '\0') {
    errno = EINVAL;
    return false;
  }
  /* Check if the sequence is valid */
  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] != '0' && s[i] != '1' && s[i] != '2') {
      errno = EINVAL;
      return false;
    }
  }
  return true;
}

/* Helper function for seq_add, that assigns the memory for the new sequence
   if it is not already in the set. */
seq_t *seq_add_helper(seq_t *p, char const *s, int i,
                      seq_t **first_added_branch) {
  if (p == NULL) {
    p = seq_new();
    p->seq = malloc((i + 2) * sizeof(char));
    if (p->seq == NULL)
      return NULL;
    strncpy(p->seq, s, i + 1);
    (p->seq)[i + 1] = '\0';
    if (*first_added_branch == NULL)
      *first_added_branch = p;
  }
  return p;
}

/* Checks if the memory allocation was successful */
int check_null(seq_t *p, seq_t **first_added_branch) {
  if (p == NULL) {
    /* Free all the memory that was allocated for the new sequence that failed
     * to allocate fully */
    seq_t *delete = *first_added_branch;
    seq_delete(delete);
    *first_added_branch = NULL;
    errno = ENOMEM;
    /* Memory allocation failed */
    return 1;
  }
  /* Memory allocation was successful */
  return 0;
}

/* Adds a new sequence and all of its prefixes to the set */
int seq_add(seq_t *p, char const *s) {
  /* Check if the parameters are valid */
  if (!check_params(p, s))
    return -1;
  seq_t *first_added_branch = NULL;
  /* Add the sequence and all of its prefixes to the set */
  for (int i = 0; s[i] != '\0'; i++) {
    if (s[i] == '0') {
      p->left = seq_add_helper(p->left, s, i, &first_added_branch);
      /* Check if the memory allocation was successful */
      if (check_null(p->left, &first_added_branch))
        return -1;
      p = p->left;
    } else if (s[i] == '1') {
      p->mid = seq_add_helper(p->mid, s, i, &first_added_branch);
      if (check_null(p->mid, &first_added_branch))
        return -1;
      p = p->mid;
    } else if (s[i] == '2') {
      p->right = seq_add_helper(p->right, s, i, &first_added_branch);
      if (check_null(p->right, &first_added_branch))
        return -1;
      p = p->right;
    }
  }
  if (first_added_branch)
    return 1;
  else
    return 0;
}

/* Removes a sequence from the set */
int seq_remove(seq_t *p, char const *s) {
  /* Check if the parameters are valid */
  if (!check_params(p, s))
    return -1;
  /* Check if the sequence is in the set */
  int i;
  for (i = 0; s[i + 1] != '\0'; i++) {
    if (p == NULL)
      return 0;
    if (s[i] == '0') {
      p = p->left;
    } else if (s[i] == '1') {
      p = p->mid;
    } else if (s[i] == '2') {
      p = p->right;
    }
  }
  /* Remove the sequence from the set */
  if (p == NULL) {
    return 0;
  } else {
    seq_t *tmp = NULL;
    if (s[i] == '0') {
      if (tmp == NULL)
        return 0;
      tmp = p->left;
      p->left = NULL;
      seq_delete(tmp);
    } else if (s[i] == '1') {
      tmp = p->mid;
      if (tmp == NULL)
        return 0;
      p->mid = NULL;
      seq_delete(tmp);
    } else if (s[i] == '2') {
      tmp = p->right;
      if (tmp == NULL)
        return 0;
      p->right = NULL;
      seq_delete(tmp);
    }
    return 1;
  }
}

/* Checks if a sequence is in the set */
int seq_valid(seq_t *p, char const *s) {
  /* Check if the parameters are valid */
  if (!check_params(p, s))
    return -1;
  /* Check if the sequence is in the set */
  for (int i = 0; s[i] != '\0'; i++) {
    if (p == NULL)
      return 0;
    if (s[i] == '0') {
      p = p->left;
    } else if (s[i] == '1') {
      p = p->mid;
    } else if (s[i] == '2') {
      p = p->right;
    }
  }
  if (p == NULL) {
    return 0;
  } else {
    return 1;
  }
}

/* Sets the equivalence class name of a sequence */
int seq_set_name(seq_t *p, char const *s, char const *n) {
  /* Check if the parameters are valid */
  if (!check_params(p, s) || n == NULL || n[0] == '\0') {
    errno = EINVAL;
    return -1;
  }
  /* Check if the sequence is in the set */
  for (int i = 0; s[i] != '\0'; i++) {
    if (p == NULL)
      return 0;
    if (s[i] == '0') {
      p = p->left;
    } else if (s[i] == '1') {
      p = p->mid;
    } else if (s[i] == '2') {
      p = p->right;
    }
  }
  /* If the sequence is in the set, set the name */
  if (p == NULL) {
    return 0;
  } else {
    /* If the name is different than the current one, set it */
    if (p->class_name == NULL || strcmp(p->class_name, n) != 0) {
      if (p->class_name != NULL)
        free(p->class_name);
      p->class_name = malloc((strlen(n) + 1) * sizeof(char));
      CHECK_NULL1(p->class_name);
      strcpy(p->class_name, n);
      p->class_name[strlen(n)] = '\0';
      return 1;
    } else
      return 0;
  }
}

/* Gets the equivalence class name of a sequence */
char const *seq_get_name(seq_t *p, char const *s) {
  /* Check if the parameters are valid */
  if (!check_params(p, s))
    return NULL;
  /* Check if the sequence is in the set */
  for (int i = 0; s[i] != '\0'; i++) {
    /* If the sequence is not in the set, return NULL */
    if (p == NULL) {
      errno = 0;
      return NULL;
    }
    if (s[i] == '0') {
      p = p->left;
    } else if (s[i] == '1') {
      p = p->mid;
    } else if (s[i] == '2') {
      p = p->right;
    }
  }
  if (p == NULL || p->class_name == NULL || p->class_name[0] == '\0') {
    errno = 0;
    return NULL;
  } else { /* If the sequence is in the set, return the name */
    return p->class_name;
  }
}

/* Helper function for seq_equiv, that implements the actual
   connection of two equivalence classes */
int seq_equiv_helper(seq_t *p1, seq_t *p2) {
  /* If both sequences are NULL, return 1 */
  if (p1->class_name == NULL && p2->class_name == NULL) {
    p1->class_name = malloc(sizeof(char));
    p2->class_name = malloc(sizeof(char));
    CHECK_NULL1(p1->class_name);
    CHECK_NULL1(p2->class_name);
    p1->class_name[0] = '\0';
    p2->class_name[0] = '\0';
    return 1;
    /* If one of the sequences is NULL, copy the name of the other one */
  } else if (p1->class_name == NULL && p2->class_name != NULL) {
    p1->class_name = malloc((strlen(p2->class_name) + 1) * sizeof(char));
    CHECK_NULL1(p1->class_name);
    strcpy(p1->class_name, p2->class_name);
    p1->class_name[strlen(p2->class_name)] = '\0';
    return 1;
  } else if (p1->class_name != NULL && p2->class_name == NULL) {
    p2->class_name = malloc((strlen(p1->class_name) + 1) * sizeof(char));
    CHECK_NULL1(p2->class_name);
    strcpy(p2->class_name, p1->class_name);
    p2->class_name[strlen(p1->class_name)] = '\0';
    return 1;
    /* If both sequences have the same name, return 0 */
  } else if (strcmp(p1->class_name, p2->class_name) == 0) {
    return 0;
  } else { /* If the sequences have different names, concatenate them */
    char *s = malloc((strlen(p1->class_name) + strlen(p2->class_name) + 1) *
                     sizeof(char));
    CHECK_NULL1(s);
    strcpy(s, strcat(p1->class_name, p2->class_name));
    s[strlen(p1->class_name) + strlen(p2->class_name)] = '\0';
    free(p1->class_name);
    free(p2->class_name);
    p1->class_name = malloc(strlen(s) * sizeof(char));
    CHECK_NULL1(p1->class_name);
    p2->class_name = malloc(strlen(s) * sizeof(char));
    CHECK_NULL1(p2->class_name);
    strcpy(p1->class_name, s);
    strcpy(p2->class_name, s);
    p1->class_name[strlen(s)] = '\0';
    p2->class_name[strlen(s)] = '\0';
    return 1;
  }
}

/* Connects two equivalence classes represented by
   two sequences into one class */
int seq_equiv(seq_t *p, char const *s1, char const *s2) {
  /* Check if the parameters are valid */
  if (!check_params(p, s1) || !check_params(p, s2)) {
    errno = EINVAL;
    return -1;
  }
  seq_t *root = p;
  /* Looks for the first sequence in the set */
  for (int i = 0; s1[i] != '\0'; i++) {
    if (p == NULL)
      return 0;
    if (s1[i] == '0') {
      p = p->left;
    } else if (s1[i] == '1') {
      p = p->mid;
    } else if (s1[i] == '2') {
      p = p->right;
    }
  }
  if (p == NULL)
    return 0;
  seq_t *p1 = p;

  p = root;
  /* Looks for the second sequence in the set */
  for (int i = 0; s2[i] != '\0'; i++) {
    if (p == NULL)
      return 0;
    if (s2[i] == '0') {
      p = p->left;
    } else if (s2[i] == '1') {
      p = p->mid;
    } else if (s2[i] == '2') {
      p = p->right;
    }
  }
  if (p == NULL)
    return 0;
  seq_t *p2 = p;
  return seq_equiv_helper(p1, p2);
}
