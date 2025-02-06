CREATE TABLE `Users` (
  `id` integer PRIMARY KEY,
  `username` varchar(16) NOT NULL,
  `email` text UNIQUE NOT NULL,
  `password` text NOT NULL,
  `created_at` datetime NOT NULL DEFAULT (CURRENT_TIMESTAMP),
  `updated_at` datetime NOT NULL DEFAULT (CURRENT_TIMESTAMP)
);

CREATE TABLE `Status` (
  `status_id` integer PRIMARY KEY,
  `status_name` text NOT NULL
);

CREATE TABLE `Tasks` (
  `id` integer PRIMARY KEY,
  `user_id` integer,
  `title` text NOT NULL,
  `description` text,
  `limited_date` datetime,
  `status` integer NOT NULL,
  `priority` integer DEFAULT 0,
  `created_at` datetime NOT NULL DEFAULT (CURRENT_TIMESTAMP),
  `updated_at` datetime NOT NULL DEFAULT (CURRENT_TIMESTAMP),
  FOREIGN KEY (`user_id`) REFERENCES `Users` (`id`),
  FOREIGN KEY (`status`) REFERENCES `Status` (`status_id`)
);

CREATE TABLE `History` (
  `id` integer PRIMARY KEY,
  `task_id` integer NOT NULL,
  `user_id` integer NOT NULL,
  `created_at` datetime NOT NULL DEFAULT (CURRENT_TIMESTAMP),
  FOREIGN KEY (`task_id`) REFERENCES `Tasks` (`id`),
  FOREIGN KEY (`user_id`) REFERENCES `Users` (`id`)
);

CREATE TABLE `Tags` (
  `id` integer PRIMARY KEY,
  `name` text UNIQUE NOT NULL,
  `color` text
);

CREATE TABLE `TaskTags` (
  `task_id` integer,
  `tag_id` integer,
  PRIMARY KEY (`task_id`, `tag_id`),
  FOREIGN KEY (`task_id`) REFERENCES `Tasks` (`id`),
  FOREIGN KEY (`tag_id`) REFERENCES `Tags` (`id`)
);