pub mod all {
    pub fn is_palindrome(x: i32) -> bool {
        // negative numbers cannot be palindromes by given definition
        // this is simply an optimization
        if x.is_negative() {
            return false;
        }

        let digits = format!("{}", x);
        let (digits_left, digits_right) = digits.split_at(digits.len() / 2);
        
        digits_left
            .chars()
            .zip(digits_right.chars().rev())
            .all(|(x, y)| x == y)
    }
}

pub mod try_fold {
    pub fn is_palindrome(x: i32) -> bool {
        // negative numbers cannot be palindromes by given definition
        // this is simply an optimization
        if x.is_negative() {
            return false;
        }

        let digits = format!("{}", x);
        let (digits_left, digits_right) = digits.split_at(digits.len() / 2);
        
        digits_left
            .chars()
            .zip(digits_right.chars().rev())
            .try_fold((), |_, (x, y)| (x == y).then_some(()).ok_or(()))
            .is_ok()
    }
}
