#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Replace with your OpenAI API key
#define API_KEY "API KEY HERE (https://platform.openai.com/api-keys)"

// Function to handle HTTP response
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  printf("%s", (char *)ptr);
  return size * nmemb;
}

// Function to send a request to OpenAI GPT
void ask_openai(const char *prompt)
{
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();

  if (curl)
  {
    char url[256];
    snprintf(url, sizeof(url), "https://api.openai.com/v1/completions");

    char post_data[1024];
    snprintf(post_data, sizeof(post_data),
             "{\"model\": \"gpt-3.5-turbo\", \"prompt\": \"%s\", \"max_tokens\": 50}", prompt);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Authorization: Bearer " API_KEY);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }

  curl_global_cleanup();
}

int main()
{
  char input[256];
  printf("Chatbot: Hello! I'm your AI-powered chatbot. Type 'bye' to exit.\n");

  while (1)
  {
    printf("You: ");
    fgets(input, sizeof(input), stdin);

    // Remove newline character from input
    input[strcspn(input, "\n")] = 0;

    // Exit condition
    if (strcmp(input, "bye") == 0)
    {
      printf("Chatbot: Goodbye!\n");
      break;
    }

    // Send input to OpenAI GPT
    printf("Chatbot: ");
    ask_openai(input);
  }

  return 0;
}