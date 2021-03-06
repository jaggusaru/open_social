<?php

/**
 * @file
 * Builds placeholder replacement tokens for Social Like module.
 */

use Drupal\Core\Render\BubbleableMetadata;

/**
 * Implements hook_token_info().
 */
function social_like_token_info() {

  $type = [
    'name' => t('Social Like'),
    'description' => t('Tokens from the social like module.'),
  ];

  $social_like['liked_entity'] = [
    'name' => t('URL of the liked entity.'),
    'description' => t('URL of the entity the like was created for'),
  ];

  return [
    'types' => ['social_like' => $type],
    'tokens' => [
      'social_like' => $social_like,
    ],
  ];
}

/**
 * Implements hook_tokens().
 */
function social_like_tokens($type, $tokens, array $data, array $options, BubbleableMetadata $bubbleable_metadata) {
  $replacements = [];
  if ($type == 'vote' && !empty($data['message'])) {
    /** @var \Drupal\message\Entity\Message $message */
    $message = $data['message'];
    /** @var \Drupal\votingapi\Entity\Vote $vote */
    if ($vote = \Drupal\votingapi\Entity\Vote::load($message->field_message_related_object->target_id)) {
      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'liked_entity':
            $storage = \Drupal::entityManager()->getStorage($vote->getVotedEntityType());
            $entity = $storage->load($vote->getVotedEntityId());
            $replacements[$original] = $entity->toUrl()->toString();
            break;
        }
      }
    }
  }
  return $replacements;
}
